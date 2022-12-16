#version 330 core
layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

out vec2 ex_TexCoor;


void build_house(vec4 position)
{
    float scale = 50.f;

    gl_Position = position + vec4(-scale, -scale, 0.0, 0.0);    // 1:bottom-left
    ex_TexCoor = vec2(0.f, 0.f);
    EmitVertex();
    gl_Position = position + vec4(scale, -scale, 0.0, 0.0);    // 2:bottom-right
    ex_TexCoor = vec2(1.f, 0.f);
    EmitVertex();
    gl_Position = position + vec4(-scale, scale, 0.0, 0.0);    // 3:top-left
    ex_TexCoor = vec2(0.f, 1.f);
    EmitVertex();
    gl_Position = position + vec4(scale, scale, 0.0, 0.0);    // 4:top-right
    ex_TexCoor = vec2(1.f, 1.f);
    EmitVertex();

    EndPrimitive();
}

void main() {
    build_house(gl_in[0].gl_Position);
}