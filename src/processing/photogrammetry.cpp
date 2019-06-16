#include <processing/photogrammetry.hpp>
#include <processing/epipolar.hpp>
#include <processing/image.hpp>
#include <spdlog/spdlog.h>

namespace mpp
{
    photogrammetry_processor::photogrammetry_processor()
    {
        _detection_settings.octaves = 4;
        _detection_settings.feature_scales = 3;
        _detection_settings.orientation_magnitude_threshold = 0.0002f;
        _sift_cache = sift::create_cache(_detection_settings.octaves, _detection_settings.feature_scales);

        _match_settings.relation_threshold = 0.8f;
        _match_settings.similarity_threshold = 0.83f;
        _match_settings.max_match_count = 50;
    }
    void photogrammetry_processor::add_image(std::shared_ptr<image> img)
    {
        const auto [insert_iter, did_emplace] = _images.emplace(std::move(img), image_info{});
        constexpr auto max_width = 400;
        auto& imgref = *insert_iter->first;
        const float aspect = imgref.dimensions().x / imgref.dimensions().y;
        const auto w = max_width;
        const auto h = aspect * max_width;
        if (did_emplace)
            insert_iter->second.feature_points = sift::detect_features(*_sift_cache, image(imgref).resize(w, h), _detection_settings, sift::dst_system::normalized_coordinates);
    }

    void photogrammetry_processor::match_all()
    {
        std::for_each(_images.begin(), _images.end(), [&](const std::pair<std::shared_ptr<image>, image_info>& a) {
            std::for_each(std::next(_images.find(a.first)), _images.end(), [&](const std::pair<std::shared_ptr<image>, image_info>& b) {
                const auto matches = sift::match_features(a.second.feature_points, b.second.feature_points, _match_settings);
                spdlog::info("{} matches.", matches.size());
                if (matches.size() >= 8)
                {
                    auto& insert = _image_matches[a.first][b.first];
                    insert.fundamental_matrix = ransac_fundamental(sift::corresponding_points(matches));
                }
                });
            });
    }
    std::optional<glm::mat3> photogrammetry_processor::fundamental_matrix(const std::shared_ptr<image>& a, const std::shared_ptr<image>& b)
    {
        bool is_a = true;
        auto it = _image_matches.find(a);
        if (it == _image_matches.end())
        {
            it = _image_matches.find(b);
            is_a = false;
        }

        if (it == _image_matches.end())
            return std::nullopt;

        auto oit = it->second.find(is_a ? b : a);
        if (oit == it->second.end())
        {
            if (is_a)
            {
                it = _image_matches.find(b);
                if (it != _image_matches.end())
                {
                    oit = it->second.find(a);
                    if (oit != it->second.end())
                        return oit->second.fundamental_matrix;
                }
            }
            return std::nullopt;
        }
        return oit->second.fundamental_matrix;
    }
}