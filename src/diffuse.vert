#version 430

in vec3 VertexPosition;
in vec3 VertexNormal;

out vec3 LightIntensity;

uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat3 NormalMatrix;

uniform vec3 LightPosition;
uniform vec3 Kd;
uniform vec3 Ld;

void main()
{
    vec3 n = VertexNormal;
    vec3 s = normalize(LightPosition - VertexPosition);
    LightIntensity = Ld * Kd * max(dot(s, n), 0.0);

    mat4 MVP = ProjectionMatrix * ModelViewMatrix;
    gl_Position = MVP * vec4(VertexPosition, 1.0);
}