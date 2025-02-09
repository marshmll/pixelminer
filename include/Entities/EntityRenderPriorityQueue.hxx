/**
 * @file EntityRenderPriorityQueue.hxx
 * @brief Declares RenderPriorityCompare and the EntityRenderPriorityQueue type.
 */

#pragma once

#include "Entities/Entity.hxx"

/**
 * @class RenderPriorityCompare
 * @brief Compares two entities based on their vertical position. Entities with
 * smaller y coordinate (i.e, with most depth), is priorityzed over the other entity.
 * @note If the entiites have hitboxes, their center is used as the comparison
 * threshold, otherwise, the bottom of the base sprite is used.
 */
class RenderPriorityCompare
{
  public:
    /**
     * @brief Compares two entities based on their vertical position.
     * @param below The entity originally at the left in the queue.
     * @param above The entity originally at the right in the queue.
     * @return True if the first entity has smaller y position, false otherwise.
     */
    const bool operator()(std::shared_ptr<Entity> &below, std::shared_ptr<Entity> &above);
};

/**
 * @typedef EntityRenderPriorityQueue
 * @brief Declares a `std::priority_queue` of `std::shared_ptr<Entity>` that uses
 * `RenderPriorityCompare` as the priority comparator
 */
using EntityRenderPriorityQueue =
    std::priority_queue<std::shared_ptr<Entity>, std::vector<std::shared_ptr<Entity>>, RenderPriorityCompare>;
