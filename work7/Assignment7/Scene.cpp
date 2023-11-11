//
// Created by Göksu Güvendiren on 2019-05-14.
//

#include "Scene.hpp"

void Scene::buildBVH()
{
    printf(" - Generating BVH...\n\n");
    this->bvh = new BVHAccel(objects, 1, BVHAccel::SplitMethod::NAIVE);
}

Intersection Scene::intersect(const Ray &ray) const
{
    return this->bvh->Intersect(ray);
}

void Scene::sampleLight(Intersection &pos, float &pdf) const
{
    float emit_area_sum = 0;
    for (uint32_t k = 0; k < objects.size(); ++k)
    {
        if (objects[k]->hasEmit())
        {
            emit_area_sum += objects[k]->getArea();
        }
    }
    float p = get_random_float() * emit_area_sum;
    emit_area_sum = 0;
    for (uint32_t k = 0; k < objects.size(); ++k)
    {
        if (objects[k]->hasEmit())
        {
            emit_area_sum += objects[k]->getArea();
            if (p <= emit_area_sum)
            {
                objects[k]->Sample(pos, pdf);
                break;
            }
        }
    }
}

bool Scene::trace(
    const Ray &ray,
    const std::vector<Object *> &objects,
    float &tNear, uint32_t &index, Object **hitObject)
{
    *hitObject = nullptr;
    for (uint32_t k = 0; k < objects.size(); ++k)
    {
        float tNearK = kInfinity;
        uint32_t indexK;
        Vector2f uvK;
        if (objects[k]->intersect(ray, tNearK, indexK) && tNearK < tNear)
        {
            *hitObject = objects[k];
            tNear = tNearK;
            index = indexK;
        }
    }

    return (*hitObject != nullptr);
}

// Implementation of Path Tracing
Vector3f Scene::castRay(const Ray &ray, int depth) const
{
    // TO DO Implement Path Tracing Algorithm here
    Vector3f l_dir(0, 0, 0);
    Vector3f l_indir(0, 0, 0);

    // get the interaction of the ray to scene
    Intersection inter = intersect(ray);

    if (!inter.happened)
        return l_dir;

    if (inter.m->hasEmission())
        return inter.m->getEmission();

    Vector3f wo = -ray.direction;
    inter.normal = inter.normal.normalized();

    // light sample
    Intersection light_sample;
    float light_pdf;
    sampleLight(light_sample, light_pdf);
    light_sample.normal = light_sample.normal.normalized();

    // dir
    Vector3f ws = light_sample.coords - inter.coords;
    float sqr_distance = dotProduct(ws, ws);
    ws = normalize(ws);
    if (abs(intersect(Ray(inter.coords, ws)).distance - sqrt(sqr_distance)) < 0.001f)
    {
        l_dir = light_sample.emit *
                inter.m->eval(wo, ws, inter.normal) *
                dotProduct(inter.normal, ws) *
                dotProduct(light_sample.normal, -ws) /
                sqr_distance /
                light_pdf;
    }

    // indir
    if (get_random_float() < Scene::RussianRoulette)
    {
        Vector3f wi = inter.m->sample(wo, inter.normal).normalized();
        Ray r(inter.coords, wi);
        Intersection hit = intersect(r);

        if (hit.happened && !hit.m->hasEmission())
        {
            l_indir = castRay(r, depth + 1) * 
                inter.m->eval(wo, wi, inter.normal) * 
                dotProduct(wi, inter.normal) /
                inter.m->pdf(wo, wi, inter.normal) /
                Scene::RussianRoulette;
        }
    }

    return l_dir + l_indir;
}
