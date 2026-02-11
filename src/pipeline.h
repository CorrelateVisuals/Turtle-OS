#ifndef TURTLE_OS_PIPELINE_H
#define TURTLE_OS_PIPELINE_H

#include <string>
#include <vector>

struct PipelinePass {
    std::string vertex_shader;
    std::string fragment_shader;
};

class Pipeline {
public:
    Pipeline();

    const std::vector<PipelinePass> &passes() const;
    void load_from_file(const std::string &path);

private:
    std::vector<PipelinePass> _passes;
};

#endif // TURTLE_OS_PIPELINE_H
