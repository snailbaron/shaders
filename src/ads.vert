#version 430

in vec3 VertexPosition;
in vec3 VertexNormal;

out vec3 LightIntensity;

struct LightInfo
{
    vec3 Position;
    vec3 La;        // ambient intensity
    vec3 Ld;        // diffuse intensity
    vec3 Ls;        // specular intensity
};
uniform LightInfo Light;

struct MaterialInfo
{
    vec3 Ka;        // ambient reflectivity
    vec3 Kd;        // diffuse reflectivity
    vec3 Ks;        // specular reflectivity
    float Shine;    // "shininess" factor for specular reflection
};
uniform MaterialInfo Material;

uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat3 NormalMatrix;

void main()
{
    // Calculate ambient light intensity
    vec3 ambientLight = Material.Ka * Light.La;

    // Calculate diffuse light intensity
    vec3 s = normalize(Light.Position - VertexPosition);
    vec3 n = normalize(VertexNormal);
    vec3 diffuseLight = Material.Kd * Light.Ld * max(dot(s, n), 0.0);

    // Calculate specular light intensity
    vec3 r = -s + n * 2 * dot(s, n);
    mat4 viewModel = inverse(ModelViewMatrix);
    vec3 cameraPos = vec3(viewModel[3]);
    vec3 v = normalize(cameraPos - VertexPosition);
    //vec3 v = -normalize(vec3(ModelViewMatrix * vec4(VertexPosition, 1.0)));
    //vec3 v = normalize(vec3(inverse(ModelViewMatrix) * -vec4(VertexPosition, 1.0)));
    vec3 specularLight = Material.Ks * Light.Ls * pow(max(dot(r, v), 0.0), Material.Shine);

    // Calculate and pass total light intensity
    LightIntensity = ambientLight + diffuseLight + specularLight;

    // Calculate vertex position
    mat4 MVP = ProjectionMatrix * ModelViewMatrix;
    gl_Position = MVP * vec4(VertexPosition, 1.0);
}