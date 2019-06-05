#include <impls/gl43_impl.hpp>
#include <processing/sift/sift.hpp>
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
        std::ifstream file("../../res/arch.jpg", std::ios::binary | std::ios::in);
        img.load_stream(file, 1);
        std::vector<image> dog = sift::difference_of_gaussian(img, 3, 5);

        int i = 0;
        for (auto& img : dog)
        {
            std::filesystem::path out_file = "./img" + std::to_string(i++) + ".png";
            img.save_stream(std::ofstream(out_file, std::ios::binary | std::ios::out));
        }
    }
    void gl43_impl::on_update(program_state& state, seconds delta)
    {
    }
    void gl43_impl::on_end(program_state& state)
    {
    }
}