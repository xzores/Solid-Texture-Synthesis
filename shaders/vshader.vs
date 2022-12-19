#version 330 core

in vec3 vVertex;
in vec3 vertex_norm;
in vec3 vAxis;
in vec2 aTexCoord1;
in vec2 aTexCoord2;
in vec2 aTexCoord3;

uniform mat4 vModel;
uniform mat4 vView;
uniform mat4 vProjection;
uniform vec3 lpos_world;
uniform vec3 eye_normal;

out vec3 n;
out vec3 e;
out vec3 l;
out vec2 TexCoord;

void main() {
	gl_Position = vProjection * vView * vModel * vec4(vVertex, 1.0);
	n = normalize(vertex_norm);
    l = normalize(lpos_world - vVertex);
    e = eye_normal;
    if(vAxis == vec3(1.0, 0.0, 0.0) || vAxis == vec3(-1.0, 0.0, 0.0)){ // Along X-Axis
        TexCoord = vec2(aTexCoord1);
    }
    else if(vAxis == vec3(0.0, 1.0, 0.0) || vAxis == vec3(0.0, -1.0, 0.0)){ // Along Y-Axis
        TexCoord = vec2(aTexCoord2);
    }
    else if(vAxis == vec3(0.0, 0.0, 1.0) || vAxis == vec3(0.0, 0.0, -1.0)){  // Along Z-Axis
        TexCoord = vec2(aTexCoord3);
    }
}