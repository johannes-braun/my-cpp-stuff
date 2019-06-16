#include <processing/photogrammetry.hpp>
#include <processing/epipolar.hpp>
#include <processing/image.hpp>
#include <spdlog/spdlog.h>
#include <Eigen/Eigen>
#include <execution>

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
    void photogrammetry_processor::add_image(std::shared_ptr<image> img, float focal_length)
    {
        const auto [insert_iter, did_emplace] = _images.emplace(std::move(img), image_info{});
        constexpr auto max_width = 400;
        auto& imgref = *insert_iter->first;
        const float aspect = imgref.dimensions().x / imgref.dimensions().y;
        const auto w = max_width;
        const auto h = aspect * max_width;
        if (did_emplace)
        {
            insert_iter->second.feature_points = sift::detect_features(*_sift_cache, image(imgref).resize(w, h), _detection_settings, sift::dst_system::normalized_coordinates);
            insert_iter->second.camera_intrinsics = glm::mat3(1.f);
            insert_iter->second.camera_intrinsics[0][0] = focal_length;
            insert_iter->second.camera_intrinsics[1][1] = focal_length;
        }
    }

    void photogrammetry_processor::match_all()
    {
        for (auto& i : _images)
            _image_matches[i.first];

        std::for_each(std::execution::par, _images.begin(), _images.end(), [&](const std::pair<std::shared_ptr<image>, image_info>& a) {
            std::for_each(std::next(_images.find(a.first)), _images.end(), [&](const std::pair<std::shared_ptr<image>, image_info>& b) {
                const auto matches = sift::match_features(a.second.feature_points, b.second.feature_points, _match_settings);
                spdlog::info("{} matches.", matches.size());
                if (matches.size() >= 8)
                {
                    auto& insert = _image_matches[a.first][b.first];
                    insert.match_points = sift::corresponding_points(matches);
                    insert.fundamental_matrix = ransac_fundamental(insert.match_points);
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
    std::optional<glm::mat3> photogrammetry_processor::essential_matrix(const std::shared_ptr<image>& a, const std::shared_ptr<image>& b)
    {
        const auto fund = fundamental_matrix(a, b);
        if (fund)
        {
            const auto& k_a = _images[a].camera_intrinsics;
            const auto& k_b = _images[a].camera_intrinsics;
            return transpose(inverse(k_b)) * *fund * inverse(k_a);
        }
        return std::nullopt;
    }
    std::optional<glm::mat4> photogrammetry_processor::relative_matrix(const std::shared_ptr<image>& a, const std::shared_ptr<image>& b)
    {
        const auto e = essential_matrix(a, b);
        if (!e) return std::nullopt;

        const Eigen::Matrix3f& e_mat = reinterpret_cast<const Eigen::Matrix3f&>(e);
        Eigen::JacobiSVD<Eigen::Matrix3f> svd(e_mat, Eigen::ComputeFullU | Eigen::ComputeFullV);

        Eigen::Matrix3f w;
        w << 0, -1, 0,
            1, 0, 0,
            0, 0, 1;
        Eigen::Matrix3f w_inv;
        w_inv = w.transpose();
        Eigen::Matrix3f z;
        z << 0, 1, 0,
            -1, 0, 0,
            0, 0, 0;

        const Eigen::Matrix3f r = svd.matrixU() * w_inv * svd.matrixV();
        const Eigen::Matrix3f tx = svd.matrixU() * z * svd.matrixU().transpose();

        glm::mat4 trafo(reinterpret_cast<const glm::mat3&>(r));
        trafo[3][0] = tx(2, 1);
        trafo[3][1] = tx(0, 2);
        trafo[3][2] = tx(1, 0);
        trafo[3][3] = 1.f;

        return trafo;
    }
    std::vector<photogrammetry_processor::transformed_image> photogrammetry_processor::build_flat_hierarchy()
    {
        std::vector<transformed_image> imgs;
        visit_match_tree([&](const std::shared_ptr<image>& img, glm::mat4 tf) {
            imgs.emplace_back(transformed_image{ img, tf });
            });
        return imgs;
    }
}