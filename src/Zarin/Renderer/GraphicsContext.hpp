#pragma once

namespace zrn {

class GraphicsContext {
public:
    virtual ~GraphicsContext() { }
    virtual void Init() = 0;
    virtual void Destroy() = 0;
    
    virtual void SwapBuffers() = 0;
};

} // namespace zrn