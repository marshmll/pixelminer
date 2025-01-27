#pragma once

class AttributeFunctionality
{
  private:
    uint8_t health;
    uint8_t maxHealth;
    
    uint8_t hunger;
    uint8_t maxHunger;

  public:
    AttributeFunctionality(const uint8_t &max_health, const uint8_t &max_hunger);

    ~AttributeFunctionality();

    /* ACCESSORS ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

    const uint8_t getHealth() const;

    const uint8_t getHunger() const;

    const uint8_t getMaxHealth() const;

    const uint8_t getMaxHunger() const;

    /* MODIFIERS ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

    void setHealth(const uint8_t health);

    void setHunger(const uint8_t hunger);

    void setMaxHealth(const uint8_t max_health);

    void setMaxHunger(const uint8_t max_hunger);
};