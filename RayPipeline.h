#ifndef RAYPIPELINE_H
#define RAYPIPELINE_H

#include "GraphicsPipeline.h"

class RayPipeline : public u_GraphicsPipeline
{
private:
    
public:
    RayPipeline();
    ~RayPipeline();
    void createGraphicsPipeline() override;
};


#endif