#include "Core/Start.h"
#include "Soda.h"

#include "imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "AppLayer.h"

namespace Soda
{
class Playground : public App
{
public:
  Playground() : App("SodaCan") { PushLayer(new SodaCan()); }

  ~Playground() {}
};

App *CreateApp() { return new Playground(); }
} // namespace Soda