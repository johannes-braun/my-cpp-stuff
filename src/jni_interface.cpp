#ifdef __ANDROID__
#include <jni.h>
#include <processing/photogrammetry.hpp>
#include <android/bitmap.h>
#include <processing/image.hpp>
#include <memory>
#include <chrono>

mpp::photogrammetry_processor_async& as_proc(jlong ptr)
{
    return *reinterpret_cast<mpp::photogrammetry_processor_async*>(ptr);
}

extern "C" JNIEXPORT jlong JNICALL Java_com_jbraun_photogrammetry_Photogrammetry_createProcessor(
        JNIEnv *env,
        jobject /* this */)
{
    return reinterpret_cast<jlong>(new mpp::photogrammetry_processor_async());
}

extern "C" JNIEXPORT void JNICALL Java_com_jbraun_photogrammetry_Photogrammetry_destroyProcessor(
        JNIEnv *env,
        jobject /* this */,
        jlong p)
{
    delete reinterpret_cast<mpp::photogrammetry_processor_async*>(p);
}

std::shared_ptr<mpp::image> image_from_bitmap(JNIEnv* env, jobject bmp)
{
    AndroidBitmapInfo info;
    void* pixels;

    if(AndroidBitmap_getInfo(env, bmp, &info) < 0)
        return nullptr;

    if(info.format != ANDROID_BITMAP_FORMAT_RGBA_8888)
        return nullptr;

    if(AndroidBitmap_lockPixels(env, bmp, &pixels) < 0)
        return nullptr;

    auto ptr = std::make_shared<mpp::image>();
    ptr->load_empty(info.width, info.height, 4);

    const char* px = static_cast<const char*>(pixels);

    std::uint32_t append = 0;
    for(std::uint32_t pos = 0; pos < info.height * info.stride; pos += info.stride, append += info.width * 4)
        memcpy(ptr->data() + append, px + pos, info.width * 4);

    AndroidBitmap_unlockPixels(env, bmp);
    return ptr;
}

extern "C" JNIEXPORT void JNICALL Java_com_jbraun_photogrammetry_Photogrammetry_pushBitmap(JNIEnv* env, jobject self, jlong proc, jobject bmp)
{
    auto& p = as_proc(proc);
    JavaVM *vm =nullptr;
    env->GetJavaVM(&vm);

    std::shared_ptr<mpp::image> i = image_from_bitmap(env, bmp);
    const auto then = std::chrono::system_clock::now();
    p.add_image(i, 0.028f, [=, obj = env->NewGlobalRef(self), bmpref = env->NewGlobalRef(bmp)]() mutable{
        if(vm->AttachCurrentThread(&env, nullptr) != JNI_OK)
        {
            return;
        }

        jclass cls = env->GetObjectClass(obj);
        jmethodID mid = env->GetMethodID(cls, "onFeaturesDetected", "(ILandroid/graphics/Bitmap;)V");
        if(mid == nullptr)
            return;

        env->CallVoidMethod(obj, mid, int(as_proc(proc).base_processor().feature_points(i).size()), bmpref);
        env->DeleteGlobalRef(obj);
        vm->DetachCurrentThread();
    });
}

extern "C" JNIEXPORT void JNICALL Java_com_jbraun_photogrammetry_Photogrammetry_run(JNIEnv* env, jobject self, jlong proc)
{
    as_proc(proc).run();
}

extern "C" JNIEXPORT void JNICALL Java_com_jbraun_photogrammetry_Photogrammetry_detectAll(JNIEnv* env, jobject self, jlong proc)
{
    as_proc(proc).detect_all();
}
#endif