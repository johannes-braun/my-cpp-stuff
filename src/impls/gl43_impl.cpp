#include <impls/gl43_impl.hpp>
#include <processing/sift/sift.hpp>
#include <processing/image.hpp>
#include <fstream>
#include <string>

namespace mpp
{
    void gl43_impl::on_setup(program_state& state)
    {
    }
    void gl43_impl::on_start(program_state& state)
    {
        image img;
        {
            std::ifstream file("../../res/IMG_20190605_174704.jpg", std::ios::binary | std::ios::in);
            img.load_stream(file, 1);
            std::vector<image> dog = sift::sift(img, 4, 3);

            int i = 0;
            for (auto& img : dog)
            {
                std::filesystem::path out_file = "./img_x1_" + std::to_string(i++) + ".png";
                img.save_stream(std::ofstream(out_file, std::ios::binary | std::ios::out));
            }
        }
        {
            std::ifstream file("../../res/IMG_20190605_174705.jpg", std::ios::binary | std::ios::in);
            img.load_stream(file, 1);
            std::vector<image> dog = sift::sift(img, 4, 3);

            int i = 0;
            for (auto& img : dog)
            {
                std::filesystem::path out_file = "./img_x2_" + std::to_string(i++) + ".png";
                img.save_stream(std::ofstream(out_file, std::ios::binary | std::ios::out));
            }
        }
    }
    void gl43_impl::on_update(program_state& state, seconds delta)
    {
    }
    void gl43_impl::on_end(program_state& state)
    {
    }
}