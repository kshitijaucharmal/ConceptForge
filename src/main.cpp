#include "setup.hpp"

using namespace Engine;

int main() {
  ConceptForge forge;

  // Render Loop
  while (!forge.WindowShouldClose()) {
    forge.DeltaTimeCalc();
    forge.ProcessInput();
    forge.Render();
    forge.CalcProjection();
    forge.GUIManagement();
  }

  return 0;
}
