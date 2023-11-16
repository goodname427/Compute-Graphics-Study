#include <iostream>
#include <vector>

#include "CGL/vector2D.h"

#include "mass.h"
#include "rope.h"
#include "spring.h"

#define KD1 0.005 
#define KD2 0.00005

namespace CGL {

    Rope::Rope(Vector2D start, Vector2D end, int num_nodes, float node_mass, float k, vector<int> pinned_nodes)
    {
        // TODO (Part 1): Create a rope starting at `start`, ending at `end`, and containing `num_nodes` nodes.
        for (int i = 0; i < num_nodes; i++)
        {
            masses.push_back(new Mass(
                start + (end - start) * (i * 1. / (num_nodes - 1)), 
                node_mass,
                false
            ));

            if (i > 0)
            {
                springs.push_back(new Spring(
                    masses[i - 1], 
                    masses[i], 
                    k
                ));
            }
        }

        // Comment-in this part when you implement the constructor
        for (auto &i : pinned_nodes) 
        {
            masses[i]->pinned = true;
        }
    }

    void Rope::simulateEuler(float delta_t, Vector2D gravity)
    {
        for (auto &s : springs)
        {
            // TODO (Part 2): Use Hooke's law to calculate the force on a node
            Vector2D dir = s->m1->position - s->m2->position;
            double len = dir.norm();  
            Vector2D force = s->k * (len - s->rest_length) * (dir / len);

            s->m1->forces -= force;
            s->m2->forces += force;
        }

        for (auto &m : masses)
        {
            if (!m->pinned)
            {
                // TODO (Part 2): Add the force due to gravity, then compute the new velocity and position
                // TODO (Part 2): Add global damping
                Vector2D a = m->forces / m->mass + gravity - KD1 * m->velocity / m->mass;
                m->velocity += a * delta_t;
                m->position += m->velocity * delta_t;
            }

            // Reset all forces on each mass
            m->forces = Vector2D(0, 0);
        }
    }

    void Rope::simulateVerlet(float delta_t, Vector2D gravity)
    {
        for (auto &s : springs)
        {
            // TODO (Part 3): Simulate one timestep of the rope using explicit Verlet （solving constraints)
            Vector2D dir = s->m1->position - s->m2->position;
            double len = dir.norm();  
            Vector2D force = s->k * (len - s->rest_length) * (dir / len);

            s->m1->forces -= force;
            s->m2->forces += force;
        }

        for (auto &m : masses)
        {
            if (!m->pinned)
            {
                Vector2D temp_position = m->position;
                // TODO (Part 3.1): Set the new position of the rope mass
                // TODO (Part 4): Add global Verlet damping
                Vector2D a = m->forces / m->mass + gravity;
                m->position = temp_position + (1 - KD2) * (temp_position - m->last_position) + a * delta_t * delta_t;
                m->last_position = temp_position;
            }

            m->forces = Vector2D(0, 0);
        }
    }
}
