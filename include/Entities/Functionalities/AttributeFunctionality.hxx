/**
 * @file AttributeFunctionality.hxx
 * @brief Defines the AttributeFunctionality class for managing entity attributes.
 */

#pragma once

/**
 * @class AttributeFunctionality
 * @brief Handles attributes like health and hunger for an entity.
 */
class AttributeFunctionality
{
  private:
    uint8_t health;    ///< Current health of the entity.
    uint8_t maxHealth; ///< Maximum health of the entity.

    uint8_t hunger;    ///< Current hunger level of the entity.
    uint8_t maxHunger; ///< Maximum hunger level of the entity.

  public:
    /**
     * @brief Constructs an AttributeFunctionality instance.
     * @param max_health Maximum health value.
     * @param max_hunger Maximum hunger value.
     */
    AttributeFunctionality(const uint8_t &max_health, const uint8_t &max_hunger);

    /**
     * @brief Destructor for AttributeFunctionality.
     */
    ~AttributeFunctionality();

    /* ACCESSORS ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

    /**
     * @brief Gets the current health value.
     * @return Current health.
     */
    const uint8_t getHealth() const;

    /**
     * @brief Gets the current hunger value.
     * @return Current hunger level.
     */
    const uint8_t getHunger() const;

    /**
     * @brief Gets the maximum health value.
     * @return Maximum health.
     */
    const uint8_t getMaxHealth() const;

    /**
     * @brief Gets the maximum hunger value.
     * @return Maximum hunger level.
     */
    const uint8_t getMaxHunger() const;

    /* MODIFIERS ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

    /**
     * @brief Sets the current health value.
     * @param health New health value.
     */
    void setHealth(const uint8_t health);

    /**
     * @brief Sets the current hunger value.
     * @param hunger New hunger value.
     */
    void setHunger(const uint8_t hunger);

    /**
     * @brief Sets the maximum health value.
     * @param max_health New maximum health value.
     */
    void setMaxHealth(const uint8_t max_health);

    /**
     * @brief Sets the maximum hunger value.
     * @param max_hunger New maximum hunger value.
     */
    void setMaxHunger(const uint8_t max_hunger);
};
