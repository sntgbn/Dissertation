#include <shaders_buffers.h>
#define STB_IMAGE_IMPLEMENTATION    

void generateObjectBuffer(GLuint& objectVao, BlenderObj& blenderObject, GLuint& shaderProgramID, shaderLocations& shaderVariableLocations) {
    GLuint vp_vbo = 0;
    GLuint vn_vbo = 0;
    GLuint tc_vbo = 0;
    GLuint tg_vbo;
    GLuint btg_vbo;

    shaderVariableLocations.vertex_position_location = glGetAttribLocation(shaderProgramID, "vertex_position");
    shaderVariableLocations.vertex_normals_location = glGetAttribLocation(shaderProgramID, "vertex_normals");
    shaderVariableLocations.texture_coordinates_location = glGetAttribLocation(shaderProgramID, "texture_coordinate");
    shaderVariableLocations.tangents_location = glGetAttribLocation(shaderProgramID, "tangent");
    shaderVariableLocations.bitangents_location = glGetAttribLocation(shaderProgramID, "bitangent");
    // Uniform variables in shader
    shaderVariableLocations.light_position_location = glGetUniformLocation(shaderProgramID, "lightPos");
    shaderVariableLocations.view_position_location = glGetUniformLocation(shaderProgramID, "viewPos");
    shaderVariableLocations.ambient_strength_location = glGetUniformLocation(shaderProgramID, "ambientStrength");
    shaderVariableLocations.specular_strength_location = glGetUniformLocation(shaderProgramID, "specularStrength");
    shaderVariableLocations.lit_outline_thickness_location = glGetUniformLocation(shaderProgramID, "lit_outline_thickness");
    shaderVariableLocations.unlit_outline_thickness_location = glGetUniformLocation(shaderProgramID, "unlit_outline_thickness");
    shaderVariableLocations.solid_outline_color_location = glGetUniformLocation(shaderProgramID, "solid_outline_color");
    shaderVariableLocations.outline_selection_location = glGetUniformLocation(shaderProgramID, "outline_selection");
    shaderVariableLocations.wobble_distortion_location = glGetUniformLocation(shaderProgramID, "wobble_distortion");
    shaderVariableLocations.cel_shader_selection_location = glGetUniformLocation(shaderProgramID, "cel_shader_selection");
    shaderVariableLocations.texture_selection_location = glGetUniformLocation(shaderProgramID, "texture_selection");
    shaderVariableLocations.texture_luminance_location = glGetUniformLocation(shaderProgramID, "texture_luminance");
    shaderVariableLocations.paper_alpha_threshold_location = glGetUniformLocation(shaderProgramID, "paper_alpha_threshold");
    shaderVariableLocations.paper_alpha_div_location = glGetUniformLocation(shaderProgramID, "paper_alpha_div");

    glGenBuffers(1, &vp_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vp_vbo);
    glBufferData(GL_ARRAY_BUFFER, 3 * blenderObject.getNumVertices() * sizeof(float), blenderObject.getVertices(), GL_STATIC_DRAW);
            
    glGenBuffers(1, &vn_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vn_vbo);
    glBufferData(GL_ARRAY_BUFFER, 3 * blenderObject.getNumVertices() * sizeof(float), blenderObject.getNormals(), GL_STATIC_DRAW);

    glGenBuffers(1, &tc_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, tc_vbo);
    glBufferData(GL_ARRAY_BUFFER, 2 * blenderObject.getNumVertices() * sizeof(float), blenderObject.getTexcoords(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &objectVao);
    glBindVertexArray(objectVao);

    // Vertex Position
    glEnableVertexAttribArray(shaderVariableLocations.vertex_position_location);
    glBindBuffer(GL_ARRAY_BUFFER, vp_vbo);
    glVertexAttribPointer(shaderVariableLocations.vertex_position_location, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    // Vertex Normals
    glEnableVertexAttribArray(shaderVariableLocations.vertex_normals_location);
    glBindBuffer(GL_ARRAY_BUFFER, vn_vbo);
    glVertexAttribPointer(shaderVariableLocations.vertex_normals_location, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    // Texture Coordinates
    glEnableVertexAttribArray(shaderVariableLocations.texture_coordinates_location);
    glBindBuffer(GL_ARRAY_BUFFER, tc_vbo);
    glVertexAttribPointer(shaderVariableLocations.texture_coordinates_location, 2, GL_FLOAT, GL_FALSE, 0, NULL);

}

std::string readShaderSource(const std::string& fileName)
{
    std::ifstream file(fileName.c_str());
    if (file.fail()) {
        std::cout << "error loading shader called " << fileName;
        exit(1);
    }

    std::stringstream stream;
    stream << file.rdbuf();
    file.close();

    return stream.str();
}

static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
    // create a shader object
    GLuint ShaderObj = glCreateShader(ShaderType);

    if (ShaderObj == 0) {
        fprintf(stderr, "Error creating shader type %d\n", ShaderType);
        exit(0);
    }
    std::string outShader = readShaderSource(pShaderText);
    const char* pShaderSource = outShader.c_str();

    // Bind the source code to the shader, this happens before compilation
    glShaderSource(ShaderObj, 1, (const GLchar**)&pShaderSource, NULL);
    // compile the shader and check for errors
    glCompileShader(ShaderObj);
    GLint success;
    // check for shader related errors using glGetShaderiv
    glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
        fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
        exit(1);
    }
    // Attach the compiled shader object to the program object
    glAttachShader(ShaderProgram, ShaderObj);
}


//Make compileshaders modular, make shaders classes
GLuint CompileShaders(GLuint& shaderProgramID, std::string vertex_shader, std::string fragment_shader)
{
    //Start the process of setting up our shaders by creating a program ID
    //Note: we will link all the shaders together into this ID
    shaderProgramID = glCreateProgram();
    if (shaderProgramID == 0) {
        fprintf(stderr, "Error creating shader program\n");
        exit(1);
    }

    // Create two shader objects, one for the vertex, and one for the fragment shader
    AddShader(shaderProgramID, vertex_shader.c_str(), GL_VERTEX_SHADER);
    AddShader(shaderProgramID, fragment_shader.c_str(), GL_FRAGMENT_SHADER);

    GLint Success = 0;
    GLchar ErrorLog[1024] = { 0 };

    // After compiling all shader objects and attaching them to the program, we can finally link it
    glLinkProgram(shaderProgramID);
    // check for program related errors using glGetProgramiv
    glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &Success);
    if (Success == 0) {
        glGetProgramInfoLog(shaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
        exit(1);
    }

    // program has been successfully linked but needs to be validated to check whether the program can execute given the current pipeline state
    glValidateProgram(shaderProgramID);
    // check for program related errors using glGetProgramiv
    glGetProgramiv(shaderProgramID, GL_VALIDATE_STATUS, &Success);
    if (!Success) {
        glGetProgramInfoLog(shaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
        exit(1);
    }
}
#pragma endregion SHADER_FUNCTIONS