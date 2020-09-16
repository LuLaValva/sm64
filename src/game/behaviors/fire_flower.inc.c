struct ObjectHitbox sFireFlowerProjectileHitbox = {
    /* interactType:      */ 0,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 0,
    /* health:            */ 0,
    /* numLootCoins:      */ 0,
    /* radius:            */ 5,
    /* height:            */ 5,
    /* hurtboxRadius:     */ 0,
    /* hurtboxHeight:     */ 0,
};

struct ObjectHitbox sFireFlowerPowerupHitbox = {
    /* interactType:      */ INTERACT_POWERUP,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 0,
    /* health:            */ 0,
    /* numLootCoins:      */ 0,
    /* radius:            */ 50,
    /* height:            */ 50,
    /* hurtboxRadius:     */ 0,
    /* hurtboxHeight:     */ 0,
};

void bhv_fire_flower_projectile_init(void) {
    o->oForwardVel = 30.0f;
    o->oVelY = -o->oForwardVel;
    obj_set_hitbox(o, &sFireFlowerProjectileHitbox);
}

void bhv_fire_flower_projectile_loop(void) {
    cur_obj_update_floor_and_walls();
    obj_move_xyz_using_fvel_and_yaw(o);

    if (obj_attack_collided_from_other_object(o) || o->oMoveFlags & OBJ_MOVE_HIT_WALL) {
        spawn_mist_particles_with_sound(SOUND_OBJ_DEFAULT_DEATH);
        mark_obj_for_deletion(o);
    } else if (find_water_level(o->oPosX, o->oPosZ) > o->oPosY) {
        spawn_mist_particles_with_sound(SOUND_GENERAL_FLAME_OUT);
        mark_obj_for_deletion(o);
    } else if (!(o->oFloorHeight < o->oPosY)) {
        o->oVelY = o->oForwardVel;
    }

    if ((o->oVelY -= 3) < -o->oForwardVel) {
        o->oVelY = -o->oForwardVel;
    }
}

void bhv_fire_flower_powerup_init(void) {
    obj_set_hitbox(o, &sFireFlowerPowerupHitbox);
    o->oGravity = 2.4f;
}

void bhv_fire_flower_powerup_loop(void) {
    object_step();
    if (o->oDistanceToMario < 1000.0f) {
        o->oMoveAngleYaw = approach_s16_symmetric(o->oMoveAngleYaw, o->oAngleToMario, 0x140);
    }

    if (o->oInteractStatus & INT_STATUS_INTERACTED) {
        obj_mark_for_deletion(o);
    }
}