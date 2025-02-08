/**
 * @file UUID.hxx
 * @brief Declares the UUID class to generate 36 characters long Universally-Unique Identifiers (UUID);
 */

#pragma once

/**
 * @class UUID
 * @brief A utility class for generating universally unique identifiers (UUIDs).
 *
 * This class provides a static method for generating random UUIDs (version 4) in the form
 * of a 36-character string, including hyphens.
 */
class UUID
{
  public:
    /**
     * @brief Default constructor.
     *
     * Constructs a UUID object. The constructor is empty as the UUID generation method
     * is static and does not rely on the instantiation of the object.
     */
    UUID();

    /**
     * @brief Destructor.
     *
     * Destroys the UUID object. The destructor is empty as no dynamic resources are used.
     */
    ~UUID();

    /**
     * @brief Generates a random UUID (version 4).
     *
     * This function generates a UUID using random hexadecimal values, formatted as
     * a string with hyphens at the appropriate positions. The UUID is 36 characters long,
     * including the 4 hyphens.
     *
     * @return A string representing the generated UUID.
     *         The format is "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx", where "x" is a hexadecimal digit.
     */
    static const std::string generate();
};
