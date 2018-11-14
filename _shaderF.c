#version 330 core

in vec4 position;
in vec4 normal;
in vec4 color;

out vec4 out_color;

void main(){
    // if(position.y == 0){//eigenlijk met normal.y, maar die is blijkbaar 0 ofzo
    //     out_color = normal.z * vec4(0, 0, 1, 1);
    // }else {
    //     out_color = normal.z * vec4(position.y/256.0f, 0.5 + 0.5 * position.y/256.0f, position.y/256.0f, 1.0f);
    // }
    out_color = vec4(color.x/255.0f, color.y/255.0f, color.z/255.0f, color.w/255.0f);
}