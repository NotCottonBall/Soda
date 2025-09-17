#include "Core/Start.h"
#include "Soda.h"

#include "imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "AppLayer.h"

namespace Soda
{
class Soda : public App
{
public:
  Soda() : App("SodaCan") { PushLayer(new SodaCan()); }
  ~Soda() {}
};

App *CreateApp() { return new Soda(); }
} // namespace Soda