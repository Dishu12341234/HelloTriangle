#if !defined(DEBUG_PIPELINE_H)
#define DEBUG_PIPELINE_H

#include "GraphicsPipeline.h"

class DebugPipeline : public u_GraphicsPipeline
{
private:
    
public:
    DebugPipeline() = default;
    ~DebugPipeline() = default;

    void createGraphicsPipeline() override;
};


#endif // DEBUG_PIPELINE_H
