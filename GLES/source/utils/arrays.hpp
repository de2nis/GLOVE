/**
 *  @file       arrays.hpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      A simple interface is provided for handling all the accesses to
 *              the arrays of classes needed in GLOVE using the map container.
 *
 */

#ifndef __ARRAYS_HPP__
#define __ARRAYS_HPP__

#include <map>

/**
 * @brief A templated class for handling the memory allocation, indexing and
 * searching of all the different arrays of classes.
 *
 * A separate map container is created for every class that the GLOVE supports.
 * This map is used later for the creation of the various new objects,
 * their indexing and searching. The key value of the map is used as the GL
 * handle, therefore the 0 value is not permitted. The mapped value is the
 * generated object.
 */
template <class ELEMENT>
class ObjectArray {
private:
    uint32_t mCounter;                 /**< The id (key-value of the map)
                                          reserved during the creation of a new
                                          object. */
    map<uint32_t, ELEMENT *> mObjects; /**< The templated map container (one
                                          for each different class that maps
                                          id to a specific object). */
public:

    /**
    * @brief The constructor initializes the key value to a non-usable value in
    * order to be assigned later to a correct one, through the allocate method.
    */
    ObjectArray() :
    mCounter(0)
    {
    }

    /**
    * @brief The destructor removes all elements from the map container (which
    * are destroyed), leaving the container with a size of 0.
    */
    ~ObjectArray()
    {
        typename map<uint32_t, ELEMENT *>::iterator it;
        for(it = mObjects.begin(); it != mObjects.end(); it++) {
            delete it->second;
        }
        mObjects.clear();
    }

    /**
    * @brief Returns the GL handle and reserves this as the new key value.
    * @return The GL handle.
    */
    uint32_t Allocate()
    {
        return ++mCounter;
    }

    /**
    * @brief Removes from the map container a single element with the given
    * key value (element is  destroyed).
    * @param index: The GL handle of the element to be destroyed.
    */
    bool Deallocate(uint32_t index)
    {
        typename map<uint32_t, ELEMENT *>::iterator it = mObjects.find(index);
        if(it != mObjects.end()) {
            delete it->second;
            mObjects.erase(it);

            return true;
        }

        return false;
    }

    /**
     * @brief Searches the container for an element with a key equivalent to
     * index and returns it.
     * @param index: The GL handle of the element to be found or to be created.
     * @return A pointer to the element in the map.
     *
     * In case the key value is not found (thus, the element does not exist)
     * a new object is created. Consequently this method is the only way to
     * insert a new element in the map.
     */
    ELEMENT *GetObject(uint32_t index)
    {
        typename map<uint32_t, ELEMENT *>::iterator it = mObjects.find(index);
        if(it != mObjects.end()) {
            return it->second;
        } else {
            return mObjects[index] = new ELEMENT();
        }
    }

    /**
     * @brief Searches the container for an element with a key equivalent to
     * index.
     * @param index: The GL handle of the element to be found.
     * @return The decision whether the element exists or not.
     */
    bool ObjectExists(uint32_t index) const
    {
        typename map<uint32_t, ELEMENT *>::const_iterator it = mObjects.find(index);
        return it == mObjects.end() ? false : true;
    }

    /**
     * @brief Returns the GL handle of a specific element of the container.
     * @param *element: The element to be searched in the container.
     * @return The GL handle of the element.
     *
     * The container is traversed using the mapped value as the search value.
     * The GL handle is returned in case the wanted element exists, else the
     * returned value is 0.
     */
    uint32_t GetObjectId(const ELEMENT * element) const
    {
        typename map<uint32_t, ELEMENT *>::const_iterator it;
        for(it = mObjects.begin(); it != mObjects.end(); it++) {
            if(it->second == element) {
                return it->first;
            }
        }

        return ~0;
    }
};

#endif // __ARRAYS_HPP__
