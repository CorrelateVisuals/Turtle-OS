#include "pipeline.h"

Pipeline::Pipeline() : _passes() {}

const std::vector<PipelinePass> &Pipeline::passes() const {
    return _passes;
}

void Pipeline::load_from_file(const std::string &path) {
    (void)path;
}
