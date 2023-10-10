/**
 *@description: sGrapp + TBC
 *@author: Xinwei Cai
 *@date: Sept. 2023
 */

#include "TBC.hpp"
#include "TBCPlus.hpp"
#include "GraphStream.hpp"

struct sGrappTBC {
    sGrappTBC(StreamGraph& g, int64_t delta) : graph_(g), delta_(delta) {
        graph_.reset();
        count_ = std::vector<int64_t>(TYPE_SIZE);
    }

    std::vector<int64_t> operator()(int32_t uni_time_per_window, const std::vector<double>& theta, int version = 3) {
        uint32_t stream_size = graph_.edge_stream_.size();
        std::vector<int64_t> res(TYPE_SIZE, 0LL);
        std::set<int64_t> unique_timestamps;
        Graph window_graph;
        window_graph.L_ = graph_.L_;
        window_graph.U_ = graph_.U_;
        window_graph.adj_.resize(graph_.L_ + graph_.U_);
        int kth_window = 0;
        auto count_tbf = [&](int edge_count) {
            // count butterfly
            std::vector<int64_t> window_count;
            if (version == 1) {
                TBC tbc(window_graph);
                window_count = tbc(delta_);
            } else if (version == 2) {
                TBCPlus tbcp(window_graph);
                window_count = tbcp(delta_, 0);
            } else {
                TBCPlus tbcp(window_graph);
                window_count = tbcp(delta_, 1);
            }
            assert(window_count.size() == TYPE_SIZE);
            for (int k = 0; k < TYPE_SIZE; ++k) {
                res[k] += window_count[k];
            }
            if (kth_window != 0) {
                for (int k = 0; k < TYPE_SIZE; ++k) {
                    res[k] += std::pow(edge_count, theta[k]);
                }
            }
            // clear the graph
            unique_timestamps.clear();
            for (auto &vec : window_graph.adj_) {
                vec.clear();
            }
            ++kth_window;
        };
        for (int i = 0, j = 0; i < stream_size; i = j) {
            while ([&]() -> bool {
                if (j >= stream_size) return false;
                auto &edge = graph_.edge_stream_[j];
                auto ti = edge.time;
                if (unique_timestamps.size() == uni_time_per_window &&
                    !unique_timestamps.count(ti)) return false;
                unique_timestamps.insert(ti);
                window_graph.adj_[edge.from].push_back({edge.to, edge.time});
                window_graph.adj_[edge.to].push_back({edge.from, edge.time});
                return true;
            }()) {
                ++j;
            }
            // count butterfly
            count_tbf(i + 1);
        }
        if (!unique_timestamps.empty()) {
            count_tbf(stream_size);
        }
        return res;
    }

    int64_t delta_;
    std::vector<int64_t> count_;
    StreamGraph& graph_;
};
