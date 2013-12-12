
class GraphicsDevice;
class DirectX_GraphicsDevice;

#include "PrimitiveBatch.h"
#include "GraphicsResourceHandles.h"

class DirectX_PrimitiveBatch : public PrimitiveBatch
{
public:
    DirectX_PrimitiveBatch(GraphicsDevice* device);
    ~DirectX_PrimitiveBatch(void);

    void Init();

    void Begin(bool AlphaBlend = false);
    void DrawTriangles(unsigned int layer, Vector2* points, int pointLength, Matrix transform, Color c);
    void DrawLines(unsigned int layer, Vector2* points, int pointLength, Matrix transform, Color c);
    void End();
    
    void Dispose(); 

    //draw sprite batch
    void DrawBatch(int layer, PrimitiveType topology);

private:
    DirectX_GraphicsDevice* device;

    VertexShaderHandle colorVertSH;
    PixelShaderHandle colorPixSH;

    //triangle batch methods.
    bool hasBatchBuffer(unsigned int layer);
    void addBatchBuffer(unsigned int layer);
    void resetBatchBuffer(unsigned int layer);
    void resetAllBatchBuffers();

    void sendBatchToBuffers(unsigned int layer);
    
    //batch for primitive triangles
    std::vector<std::vector<VertexCol>> triBatch;       // int (layer) -> array of triangles
    std::vector<VertexBufferHandle> triBatchVBuffers;   // int (layer) -> VertexBuffer
    IndexBufferHandle triBatchIBuffer;
};