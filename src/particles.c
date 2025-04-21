#include "simple_logger.h"
#include "particles.h"
#include "gf2d_draw.h"
#include "gfc_config.h"
#include "gfc_types.h"


typedef struct
{
	Particle* particleList;
	Uint32 particleCount;
}ParticleManager;

static ParticleManager particleManager = { 0 };

void particle_system_close();

void particle_system_init(Uint32 maxParticles)
{
	if (!maxParticles)
	{
		slog("Cannot initililzee particle system for zero particles");
		return;
	}
	if (particleManager.particleList)
	{
		slog("Particle Manager already initilized");
		return;
	}
	particleManager.particleList = gfc_allocate_array(sizeof(Particle),maxParticles);
	particleManager.particleCount = maxParticles;
}

void particle_system_close()
{
	if (particleManager.particleList)free(particleManager.particleList);
	particleManager.particleList = NULL;
	particleManager.particleCount = 0;
}


Particle* particle_new()
{
	int i;
	for (i = 0; i < particleManager.particleCount; i++)
	{
		if (!particleManager.particleList[i]._inuse)
		{
			memset(&particleManager.particleList[i], 0, sizeof(Particle));
			particleManager.particleList[i]._inuse = 1;
			return &particleManager.particleList[i];
		}
	}
	return NULL;
}

void particle_free(Particle* particle)
{
	if (!particle)return;
	particle->_inuse = 0;
}

void particleDraw(Particle* particle)
{
	if (!particle)return;
	particle->ttl--;
	if (particle->ttl <= 0)
	{
		particle_free(particle);
		return;
	}
	gfc_vector2d_add(particle->position, particle->position, particle->velocity);
	gfc_vector2d_add(particle->velocity, particle->acceleration, particle->velocity);

	gfc_color_add(&particle->drawColor, particle->drawColor,particle->colorVector);

	gf2d_draw_pixel(particle->position, particle->drawColor);

}

void particle_system_draw()
{
	int i;
	for (i = 0; i < particleManager.particleCount; i++)
	{
		if (!particleManager.particleList[i]._inuse)continue;
		particleDraw(&particleManager.particleList[i]);
	}
}

void* particles_from_file(const char* filename, Uint32 count, GFC_Vector2D position, GFC_Vector2D direction, GFC_Vector2D acceleration)
{
	int i;
	SJson* json;
	json = sj_load(filename);
	if (!json)
	{
		return;
	}
	for (i = 0; i < count; i++)
	{
		particle_from_json(json, position, direction, acceleration);
	}
	sj_free(json);

}


Particle* particle_from_json(SJson* json, GFC_Vector2D position, GFC_Vector2D direction, GFC_Vector2D acceleration)
{
	float speed;
	float speedVariance = 0;
	float angleVariance = 0;
	GFC_Color colorVariance = { 0 };
	Particle* p;
	if (!json)return NULL;
	p = particle_new();
	if (!p)return NULL;

	gfc_vector2d_copy(p->position, position);
	gfc_vector2d_normalize(&direction);


	sj_object_get_float(json, "angleVarance", &angleVariance);
	angleVariance *= GFC_DEGTORAD;
	angleVariance *= gfc_crandom();
	direction = gfc_vector2d_rotate(direction, angleVariance);
	gfc_vector2d_copy(p->velocity, direction);


	sj_object_get_float(json, "speed", &speed);
	sj_object_get_float(json, "speedVariance", &speedVariance);
	speed += gfc_crandom() * speedVariance; //Positive only
	gfc_vector2d_scale(p->velocity, p->velocity, speed);
	gfc_vector2d_copy(p->acceleration, acceleration);

	sj_object_get_color_value(json, "drawColor", &p->drawColor);
	sj_object_get_color_value(json, "colorVector", &p->colorVector);
	//p->drawColor = sj_object_get_color(json, "drawColor");
	//p->colorVector = sj_object_get_color(json, "drawColor");
	slog("drawColor: %f", p->drawColor.a);
	if (sj_object_get_color_value(json, "colorVariance", &colorVariance))
	{
		colorVariance.r *= gfc_crandom();
		colorVariance.g *= gfc_crandom();
		colorVariance.b *= gfc_crandom();
		colorVariance.a *= gfc_crandom();
		gfc_color_add(&p->drawColor, p->drawColor, colorVariance);
	}
	sj_object_get_uint32(json, "ttl", p->ttl);
}
