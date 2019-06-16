#pragma once
#include <processing/image.hpp>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <optional>
#include <glm/glm.hpp>
#include <processing/sift/sift.hpp>

namespace mpp::sift
{
    struct sift_cache;
}

namespace mpp
{
    class photogrammetry_processor
    {
    public:
        photogrammetry_processor();
        
        void add_image(std::shared_ptr<image> img);
        void match_all();

        std::optional<glm::mat3> fundamental_matrix(const std::shared_ptr<image>& a, const std::shared_ptr<image>& b);

    private:
        sift::detection_settings _detection_settings;
        sift::match_settings _match_settings;
        std::shared_ptr<sift::sift_cache> _sift_cache;

        struct image_info
        {
            std::vector<sift::feature> feature_points;
        };
        std::unordered_map<std::shared_ptr<image>, image_info> _images;

        struct match_list
        {
            glm::mat3 fundamental_matrix;
        };
        std::unordered_map<std::shared_ptr<image>, std::unordered_map<std::shared_ptr<image>, match_list>> _image_matches;
    };
}