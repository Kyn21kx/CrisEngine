//NOTE(cris): Tengo que resolver el problema de saber en que OS me encuentro, supongo que usar
// CMake es la mejor opcion y directamente descartar la impleementacion de otros OS, aun asi
// que pasa si alquien quiere todas las posibles implementaciones, nunca se sabe.

//helper functions
// NOTE(cris): La idea seria hacer privada a todas estas funciones

namespace os {
	void* dlib_open(const char* library_path);

	void dlib_close(void* handle);

	void* dlib_load_symbol(void* libHandle, const char* symbolName);
}


