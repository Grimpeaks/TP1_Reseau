
#GSL from Microsoft project wide integrtation
hunter_add_package(Microsoft.GSL)
hunter_add_package(nlohmann_json)
hunter_add_package(fmt)
#Find librairies loaded by hunter
find_package(Microsoft.GSL CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
#Link the library for all the projects
link_libraries(Microsoft.GSL::GSL)
link_libraries(nlohmann_json::nlohmann_json)
link_libraries(fmt::fmt)