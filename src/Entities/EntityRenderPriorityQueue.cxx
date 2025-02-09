#include "Entities/EntityRenderPriorityQueue.hxx"
#include "stdafx.hxx"

const bool RenderPriorityCompare::operator()(std::shared_ptr<Entity> &below, std::shared_ptr<Entity> &above)
{
    if (above->getRenderBehavior() == RenderBehavior::AlwaysOnTop)
    {
        return false;
    }

    else if (below->getRenderBehavior() == RenderBehavior::AlwaysOnTop)
    {
        return true;
    }
    else if (below->getRenderBehavior() == RenderBehavior::Perspective ||
             above->getRenderBehavior() == RenderBehavior::Perspective)
    {
        const float below_pos_y = below->isCollideable()
                                      ? below->getFirstHitBoxPosition().y + below->getFirstHitBoxSize().y / 2.f
                                      : below->getPosition().y + below->getSize().y;

        const float above_pos_y = above->isCollideable()
                                      ? above->getFirstHitBoxPosition().y + above->getFirstHitBoxSize().y / 2.f
                                      : above->getPosition().y + above->getSize().y;

        return above_pos_y <= below_pos_y;
    }
    else
    {
        return false;
    }
}
