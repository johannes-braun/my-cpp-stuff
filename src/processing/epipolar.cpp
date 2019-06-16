#include <processing/epipolar.hpp>
#include <array>
#include <Eigen/Eigen>
#include <random>

namespace mpp
{
    template<typename BidiIter, typename Rng>
    BidiIter random_unique(BidiIter begin, BidiIter end, size_t num_random, Rng&& rng) {
        size_t left = std::distance(begin, end);
        while (num_random--) {
            BidiIter r = begin;
            std::uniform_int_distribution<ptrdiff_t> dis(0, left);
            std::advance(r, dis(rng));
            std::swap(*begin, *r);
            ++begin;
            --left;
        }
        return begin;
    }

    template<class Iter>
    glm::mat3 fundamental(Iter begin, Iter end)
    {
        assert(std::distance(begin, end) >= 8);
        using vec2_t = Eigen::Vector2f;

        Eigen::Matrix<float, 8, 9> a;

        for (int i = 0; i < 8; ++begin, ++i)
        {
            const auto& [xy1, xy2] = *begin;
            a.row(i) << (xy2.x * xy1.x), (xy2.x * xy1.y), xy2.x, (xy2.y * xy1.x), (xy2.y * xy1.y), xy2.y, xy1.x, xy1.y, 1.f;
        }

        Eigen::EigenSolver<Eigen::Matrix<float, 9, 9>> solver;
        solver.compute(a.transpose() * a, true);

        int imin = -1;
        float vmin = std::numeric_limits<float>::max();
        for (int i = 0; i < solver.eigenvalues().rows(); ++i)
        {
            auto real = solver.eigenvalues()[i].real();
            if (real < vmin)
            {
                vmin = real;
                imin = i;
            }
        }
        const Eigen::Matrix<float, 9, 1> v = solver.eigenvectors().real().col(imin);
        Eigen::Matrix<float, 3, 3, Eigen::ColMajor> f;
        f.row(0) = v.segment<3>(0);
        f.row(1) = v.segment<3>(3);
        f.row(2) = v.segment<3>(6);

        Eigen::JacobiSVD<decltype(f)> svd(f, Eigen::ComputeFullU | Eigen::ComputeFullV);
        Eigen::Matrix3f s = svd.singularValues().asDiagonal();
        s(2, 2) = 0.f;
        f = svd.matrixU() * s * svd.matrixV().transpose();
        return reinterpret_cast<const glm::mat3&>(f);
    }
    glm::mat3 ransac_fundamental(std::vector<std::pair<glm::vec2, glm::vec2>> matches)
    {
        float min_cost = std::numeric_limits<float>::max();
        glm::mat3 best_mat;
        std::mt19937 rng;
        assert(matches.size() >= 8);

        for (int i = 0; i < 8; ++i)
        {
            std::shuffle(matches.begin(), matches.end(), rng);
            const auto f = fundamental(matches.begin(), matches.begin() + 8);

            float cost = 0.f;
            for (int i = 0; i < matches.size(); ++i)
            {
                auto a = glm::vec3(matches[i].first, 1);
                auto b = glm::vec3(matches[i].second, 1);
                auto diff = dot(b, f * a); // x2^T * F * x1;

                if(abs(diff) > 1e-7f)
                    cost += 1.f;
            }
            if (cost < min_cost)
            {
                min_cost = cost;
                best_mat = f;
            }
        }
        return best_mat;
    }
}