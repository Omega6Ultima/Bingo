// Dustin Gehm

#include "ScriptManager.h"

#if BINGO_SCRIPT_LANG_PY
using Bingo::Script::Python::ScriptManager_Py;

PYBIND11_MODULE(PyBingo, m) {
	m.doc() = "Bingo Module exposing C Functions to Python";
}
#endif