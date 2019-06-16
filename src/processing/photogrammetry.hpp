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
        struct transformed_image
        {
            std::shared_ptr<image> img;
            glm::mat4 transformation;
        };

        photogrammetry_processor();
        
        void add_image(std::shared_ptr<image> img, float focal_length);
        void match_all();

        std::optional<glm::mat3> fundamental_matrix(const std::shared_ptr<image>& a, const std::shared_ptr<image>& b);
        std::optional<glm::mat3> essential_matrix(const std::shared_ptr<image>& a, const std::shared_ptr<image>& b);
        std::optional<glm::mat4> relative_matrix(const std::shared_ptr<image>& a, const std::shared_ptr<image>& b);
        std::vector<transformed_image> build_flat_hierarchy();

    private:
        template<typename Visitor>
        void visit_match_tree_impl(Visitor&& vis, const std::shared_ptr<image>& img, glm::mat4 tf, std::unordered_set<image*>& visited)
        {
            if (const auto it = visited.find(img.get()); it == visited.end())
            {
                vis(img, tf);
                visited.emplace(img.get());
                for (const auto& cp : _image_matches[img])
                {
                    const auto rel = *relative_matrix(img, cp.first);
                    visit_match_tree_impl<Visitor>(std::forward<Visitor>(vis), cp.first, rel * tf, visited);
                }
            }
        }
        template<typename Visitor>
        void visit_match_tree(Visitor&& vis)
        {
            // Find the image with the most correspondences.
            auto largest = _image_matches.begin();
            size_t lsize = 0;
            for (auto it = _image_matches.begin(); it != _image_matches.end(); ++it)
            {
                if (it->second.size() > lsize)
                {
                    largest = it;
                    lsize = it->second.size();
                }
            }

            std::unordered_set<image*> visited;
            vis(largest->first, glm::mat4(1.f));
            visited.emplace(largest->first.get());
            for (const auto& cp : _image_matches[largest->first])
            {
                const auto rel = *relative_matrix(largest->first, cp.first);
                visit_match_tree_impl<Visitor>(std::forward<Visitor>(vis), cp.first, rel, visited);
            }
        }

        sift::detection_settings _detection_settings;
        sift::match_settings _match_settings;
        std::shared_ptr<sift::sift_cache> _sift_cache;

        struct image_info
        {
            std::vector<sift::feature> feature_points;
            glm::mat3 camera_intrinsics;
        };
        std::unordered_map<std::shared_ptr<image>, image_info> _images;

        struct match_list
        {
            glm::mat3 fundamental_matrix;
            std::vector<std::pair<glm::vec2, glm::vec2>> match_points;
        };
        std::unordered_map<std::shared_ptr<image>, std::unordered_map<std::shared_ptr<image>, match_list>> _image_matches;
    };
}