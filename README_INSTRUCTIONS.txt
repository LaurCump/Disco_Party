Miscarea dansatorilor se foloseste de asemenea de coliziuni pentru a nu exista cazuri in care un dansator intra prin alt dansator. Astfel la o coliziune, dansatorii isi vor schimba directia ca si cum ar fi o bila ce a lovit un perete.

Iluminarea emisa de suprafata ringului de dans 
	Desenarea gridului de celule pentru ringul de dans are loc in level.cpp, functia void::Level CreateDanceFloor() si este o varianta simplificata al crearii labirintului din proiectul 2.
	Pentru calcularea si trimiterea spre shader doar a luminilor care au influenta asupra unui obiect desenat retin in fiecare dansator un tile pe care se afla si il actualizez in functie de pozitia dansatorului raportat la tile-ul respectiv. Acest lucru se poate vedea in level.cpp, functia UpdateDancers(float deltaTimeSeconds).

Iluminarea ce provine de la reflectoare
	Calcularea iluminarii a cel putin 4 surse de lumina spot, in fragment shader, pentru dansatori, suprafata ringului de dans si ziduri se realizeaza in fragment .
	Pentru desenarea conurilor de lumina avem functia din game.h si game.cpp Mesh* CreateCone(std::string name), unde creez conul mutand centrul cercului mai sus. Dupa care acestea se deseneaza in functia void RenderSpotlights().
	Pentru rotatia unui con de tip spotlight a trebuit sa fac rotatia cu unghiuri in radiani fata de varful de sus ce se vede in: void RenderSpotlights() din fisierul game.cpp 
	Pentru a calcula unghiurile respective ne vom uita in level.cpp functia UpdateSpotlights() ce va face trimitere la fisierul world_light.cpp unde avem functii de miscare si schimbare a directiei aproape identice cu cele din dancer.cpp.
	In plus totusi, vom avea de inteles ce se afla in functia Move din world_light.cpp intrucat acolo se calculeaza
	efectiv acele unghiuri. Astfel o formula ce se face remarcata este:
	newLightDirection = glm::vec3(glm::rotate(glm::mat4(1), -radiansAngle, glm::vec3(0, 0, 1)) * glm::vec4(newLightDirection, 1))

Iluminarea globului disco
	Generarea texturii contine o culoare aleasa aleator pentru fiecare pixel.
	Pentru calcularea iluminarii in fragment shader pentru ziduri, dansatori, suprafata ringului de dans, tavan si glob disco, fac mix intre textura globului disco si culorile zidurilor, dansatorilor, suprafata ringului de dans, tavan si glob disco.
	Pentru rotatia continua a globului disco, realizam translatia coordonatelor de textura.

Switch intre cele 3 tipuri de iluminare la apasarea unei taste
	Folosesc 3 boolene in game.cpp:
bool discoLightActive;
bool floorLightActive;
bool spotlightActive;
	Pe acestea le controlez cu tastele 1,2 respectiv 3 in functia OnKeyPress din game.cpp.

Deplasarea aleatoare a dansatorilor
	Pentru acest lucru am creat functii in dancer.cpp.

In functia void RenderSimpleMesh(Mesh* mesh, Shader* shader, std::vector<World_light*> world_lights, const glm::mat4 &modelMatrix, Texture2D* texture, bool textureMoving = false) am vrut sa fie folosita de catre un obiect ce trebuie randat cu o textura precum mingea de disco.
Iar in functia void RenderSimpleMesh(Mesh *mesh, Shader* shader, std::vector<World_light*> world_lights, const glm::mat4 &modelMatrix, const World_object::Material& material) am vrut sa randez restul obiectelor ce au doar o culoare (se aflta in Material) si ce sunt luminate de surse sau ce lumineaza obiecte. 

Se apeleaza functia urmatoare in ambele functii de mai sus pentru a reduce din cod duplicat:
void SetShaderLightProperties(Shader* shader, std::vector<World_light*> world_lights, const World_object::Material& material)

Se poate observa un lucru nou "world_lights". Acest vector contine sursele de lumina ce lumineaza obiectul ce a apelat functia de randare.
Se poate observa de asemenea in FragmentShader ca pentru fiecare lumina ce a fost trimisa din acel vector "world_lights", se calculeaza o culoare ce este adaugata in continuare la culoarea finala a obiectului ce vrea sa fie randat. 