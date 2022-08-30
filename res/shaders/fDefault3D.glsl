#version 430

struct TextureMaterial {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct PointLight {
    vec4 color;
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

struct DirectionLight
{
    vec4 color;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 16 
uniform PointLight      point_lights[NR_POINT_LIGHTS];
uniform DirectionLight  dirLight;
uniform TextureMaterial material;
uniform vec3            camera_position;
uniform int             points_lights_count;

in vec2                 Texture_coord;
in vec3                 Normal;
in vec3                 Fragment_pos;

out vec4                Fragment_color;

vec3 CalcDirLight(DirectionLight light, vec3 normal, vec3 view_direction)
{
    vec3 light_direction = normalize(-light.direction);
    // диффузное освещение
    float diff = max(dot(normal, light_direction), 0.0);
    // освещение зеркальных бликов
    vec3 reflect_direction = reflect(-light_direction, normal);
    float spec = pow(max(dot(view_direction, reflect_direction), 0.0), material.shininess);
    // комбинируем результаты
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, Texture_coord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, Texture_coord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, Texture_coord));
    return (ambient + diffuse + specular) * vec3(light.color.r, light.color.g, light.color.b);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragment_position, vec3 view_direction)
{
    vec3 light_direction = normalize(light.position - fragment_position);
    // диффузное освещение
    float diff = max(dot(normal, light_direction), 0.0);
    // освещение зеркальных бликов
    vec3 reflec_direction = reflect(-light_direction, normal);
    float spec = pow(max(dot(view_direction, reflec_direction), 0.0), material.shininess);
    // затухание
    float distance    = length(light.position - fragment_position);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // комбинируем результаты
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, Texture_coord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, Texture_coord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, Texture_coord));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular) * vec3(light.color.r, light.color.g, light.color.b);
} 

void main() {
        // свойства
    vec3 norm = normalize(Normal);
    vec3 view_direction = normalize(camera_position - Fragment_pos);

    // фаза 1: Направленный источник освещения
    vec3 result = CalcDirLight(dirLight, norm, view_direction);
    // фаза 2: Точечные источники
    for(int i = 0; i < points_lights_count; i++)
        result += CalcPointLight(point_lights[i], norm, Fragment_pos, view_direction);    
    // фаза 3: фонарик
    //result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    
    
    Fragment_color = vec4(result, 1.0);
}