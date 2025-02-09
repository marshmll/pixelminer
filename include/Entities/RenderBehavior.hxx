/**
 * @file RenderBehavior.hxx
 * @brief Defines render behaviors for entities.
 */

#pragma once

/**
 * @enum RenderBehavior
 * @brief Indicates how an entity should be rendered relative to others.
 */
enum RenderBehavior : uint8_t
{
    Flat,        ///< Renders as a flat sprite. Rendering order matters.
    AlwaysOnTop, ///< Render on top of the others. If two entities have this behavior, rendering order matters.
    Perspective, ///< The rendering order is adjusted based on a vertical coordinate threshold.
};