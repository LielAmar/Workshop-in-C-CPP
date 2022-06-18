#ifndef HASH_MAP_HPP
#define HASH_MAP_HPP

#define DEFAULT_CAPACITY 16
#define CAPACITY_CHANGER 2
#define LOWER_BOUND ((double) 1 / 4)
#define UPPER_BOUND ((double) 3 / 4)
#define ITERATOR_END_VALUE -1

// ===== Messages =====
#define LENGTH_ERROR_KEYS_VALUES_SIZE \
  "The length of Keys and Values lists are not matching!"
#define VALUE_WITH_THAT_KEY_DOESNT_EXIST \
  "A value with the given key does not exist!"

#include <algorithm>
#include <stdexcept>
#include <vector>

template <class KeyT, class ValueT>
class HashMap
{
  public:
    HashMap ()
    {
      // TODO: ask ofek if what happens when we insert the first element
      // because then we need to change the capacity because it's less than 1/4
      this->init (DEFAULT_CAPACITY, 0);
    }

    HashMap (const std::vector<KeyT> keys, const std::vector<ValueT> values)
        : HashMap ()
    {
      if (keys.size () != values.size ())
      {
        throw std::length_error (LENGTH_ERROR_KEYS_VALUES_SIZE);
      }

      // TODO: Might need to change HashMap() to the line under this if the
      // variables are not freed when an exception is thrown
      // this->init ();

      for (size_t i = 0; i < keys.size (); i++)
      {
        this->insertion (keys[i], values[i], true);
      }

      // Since we added all items with #insertion and not #insert, we need to
      // privately take care of resize
      this->resize_up ();
    }

    HashMap (const HashMap<KeyT, ValueT> &hash_map) : HashMap ()
    {
      (*this) = hash_map;
    }

    virtual ~HashMap ()
    {
      delete[] this->_data;
    }

    // ===== Getters =====
    int size () const
    {
      return this->_size;
    }

    int capacity () const
    {
      return this->_capacity;
    }

    bool empty () const
    {
      return this->size () == 0;
    }

    /**
     * Checks if our HashMap contains the key ${key} by looping over all
     * buckets and checking if there's an entry (node) with that key
     *
     * @param key Key to check
     */
    bool contains_key (const KeyT &key) const
    {
      const entry *ent = this->find_by_key (key);
      return (ent != nullptr);
    }

    int bucket_index (const KeyT &key) const
    {
      if (!this->contains_key (key))
      {
        throw std::invalid_argument (VALUE_WITH_THAT_KEY_DOESNT_EXIST);
      }

      return this->hash (key);
    }

    int bucket_size (const KeyT &key) const
    {
      if (!this->contains_key (key))
      {
        throw std::invalid_argument (VALUE_WITH_THAT_KEY_DOESNT_EXIST);
      }

      return this->_data[this->hash (key)].size ();
    }

    double get_load_factor () const
    {
      return (double) this->_size / this->_capacity;
    }

    // ===== Functions =====
    /**
     * Inserts ${key, value} into the HashMap
     * @param key key to use
     * @param value value to use
     * @return returns true if the insertion was successful, false otherwise
     */
    bool insert (const KeyT &key, ValueT /* TODO: might need to be & */ value)
    {
      bool result = this->insertion (key, value);

      if (!result)
      {
        return false;
      }

      // If the insertion was successful, we want to call #resize to check
      // whether we hit the upper bound. If so, we want to resize the
      // bucket
      this->resize_up ();
      return result;
    }

    /**
     * Returns a const reference to the value assigned to ${key}
     * @param key key to use to search
     * @return a reference to the value matching the given key
     * @throws an invalid_argument exception if no such key exists
     */
    const ValueT &at (const KeyT &key) const
    {
      const entry *ent = this->find_by_key (key);

      if (ent == nullptr)
      {
        throw std::invalid_argument (VALUE_WITH_THAT_KEY_DOESNT_EXIST);
      }

      return (*ent).second;
    }

    ValueT &at (const KeyT &key)
    {
      entry *ent = this->find_by_key (key);

      if (ent == nullptr)
      {
        throw std::invalid_argument (VALUE_WITH_THAT_KEY_DOESNT_EXIST);
      }

      return (*ent).second;
    }

    /**
     * Removes a single item from the HashMap
     * @param key removes the item with key ${key}
     * @return returns true if removal was successful, false otherwise
     */
    virtual bool erase (const KeyT &key)
    {
      entry *ent = this->find_by_key (key);

      if (ent == nullptr)
      {
        return false;
      }

      int index = this->hash (key);
      bucket &buck = this->_data[index];

      // for (typename bucket::iterator iter = buck.begin (); iter != buck.end
      // ();
      //      ++iter)
      // {
      //   if (*iter == *ent)
      //   {
      //     buck.erase (iter);
      //     break;
      //   }
      // }

      buck.erase (std::remove (buck.begin (), buck.end (), *ent), buck.end ());
      this->_size--;

      this->resize_down (); // TODO: Check if we do need to call #resize here
                            // since it won't be O(n) anymore
      return true;
    }

    /**
     * Clears the data structure - removes all entries and resets values
     * Keeps the old capacity.
     */
    void clear ()
    {
      delete[] this->_data;

      this->init (this->_capacity, 0);
    }

    // ===== Operators =====
    HashMap<KeyT, ValueT> &operator= (const HashMap<KeyT, ValueT> &hash_map)
    {
      if (*this == hash_map)
      {
        return (*this);
      }

      if (this->_capacity != hash_map._capacity)
      {
        delete[] this->_data;

        this->init (hash_map._capacity, 0);
      }

      this->_size = hash_map._size;
      for (int i = 0; i < this->_capacity; i++)
      {
        this->_data[i] = hash_map._data[i]; // TODO: might be a problem since
                                            // pointing to the same place
      }

      return (*this);
    }

    ValueT &operator[] (const KeyT &key)
    {
      entry *ent = this->find_by_key (key);

      if (ent != nullptr)
      {
        return (*ent).second;
      }

      ValueT val = ValueT ();
      this->insert (key, val);

      return this->find_by_key (key)->second;
    }

    const ValueT &operator[] (const KeyT &key) const
    {
      // TODO: make sure this doesn't lead to infinite recursion
      ValueT &val = (*this)[key];
      return val;
    }

    friend bool operator== (const HashMap<KeyT, ValueT> &a,
                            const HashMap<KeyT, ValueT> &b)
    {
      if (a._size != b._size)
      {
        return false;
      }

      for (const entry &item : a)
      {
        if (!b.contains_key (item.first) || b.at (item.first) != item.second)
        {
          return false;
        }
      }

      for (const entry &item : b)
      {
        if (!a.contains_key (item.first) || a.at (item.first) != item.second)
        {
          return false;
        }
      }

      return true;
    };

    friend bool operator!= (const HashMap<KeyT, ValueT> &a,
                            const HashMap<KeyT, ValueT> &b)
    {
      return !(a == b);
    };

    // ===== Iterator =====
    class ConstIterator;
    typedef ConstIterator const_iterator;

    ConstIterator begin () const
    {
      return ConstIterator (&(*this));
    }

    ConstIterator end () const
    {
      return ConstIterator (&(*this), ITERATOR_END_VALUE, ITERATOR_END_VALUE);
    }

    ConstIterator cbegin () const
    {
      return ConstIterator (&(*this));
    }

    ConstIterator cend () const
    {
      return ConstIterator (&(*this), ITERATOR_END_VALUE, ITERATOR_END_VALUE);
    }

  protected:
    typedef std::pair<KeyT, ValueT> entry;
    typedef std::vector<entry> bucket;

    void resize_up ()
    {
      int optimal_capacity = this->optimal_capacity_up (this->_size);

      if (this->_capacity >= optimal_capacity)
      {
        return;
      }

      this->resize (optimal_capacity);
    }

    void resize_down ()
    {
      int optimal_capacity = this->optimal_capacity_down (this->_size);

      if (this->_capacity <= optimal_capacity)
      {
        return;
      }

      this->resize (optimal_capacity);
    }

    /**
     * Checks if the data structure's size is optimal. If not, it resizes it
     * and rehashes all elements.
     */
    void resize (int optimal_capacity)
    {
      int old_cap = this->_capacity;
      bucket *old_data = this->_data;

      this->init (optimal_capacity, 0);

      for (int i = 0; i < old_cap; i++)
      {
        bucket &buck = old_data[i];

        for (entry &ent : buck)
        {
          this->insertion (ent.first, ent.second);
        }
      }

      delete[] old_data;
    }

    /**
     * Searches the data structure for an element with key ${key}.
     * If no such element was found, we return nullptr
     * @param key key to use to search
     * @return returns a pointer to a pair of <KeyT, ValueT> with key ${key}.
     */
    entry *find_by_key (const KeyT &key)
    {
      int index = this->hash (key);

      bucket &buck = this->_data[index];
      for (entry &ent : buck)
      {
        if (ent.first == key)
        {
          return &ent;
        }
      }

      return nullptr;
    }

    const entry *find_by_key (const KeyT &key) const
    {
      int index = this->hash (key);

      bucket &buck = this->_data[index];
      for (const entry &ent : buck)
      {
        if (ent.first == key)
        {
          return &ent;
        }
      }

      return nullptr;
    }

    /**
     * Handles the insertion of an element to the data structure.
     * @param key key to use
     * @param value value of the element
     * @param should_replace whether we should replace if an item with key
     * ${key} already exists
     * @return whether the insertion was successful
     */
    bool insertion (const KeyT key, ValueT value, bool should_replace = false)
    {
      entry *ent = this->find_by_key (key);

      if (ent != nullptr)
      {
        if (should_replace)
        {
          (*ent).second = value;
          return true;
        }

        return false;
      }

      int index = this->hash (key);

      bucket &buck = this->_data[index];
      buck.emplace_back (key, value);

      this->_size++;
      return true;
    }

  private:
    std::hash<KeyT> _hash_func;

    int _capacity;
    int _size;

    bucket *_data;

    /**
     * Initializes the HashMap to capacity ${capacity} and size of ${size}
     */
    void init (int capacity = DEFAULT_CAPACITY, int size = 0)
    {
      this->_hash_func = std::hash<KeyT>{};

      this->_capacity = capacity;
      this->_size = size;
      this->_data = new bucket[this->_capacity];

      // Setting the size of every bucket to be 0 initally to prevent errors
      // with garbage value when calling #resize later on
      for (int i = 0; i < this->_capacity; i++)
      {
        this->_data[i] = bucket (0);
      }
    }

    /**
     * Calculates the optimal capacity for M elements using the upper and lower
     * bounds
     * @param M number of elements that are going to be stored
     * @return the optimal capacity for M elements
     */
    int optimal_capacity_up (long M) const
    {
      if (M == 0)
      {
        return 1;
      }

      double factor = ((double) M / this->_capacity);

      if (factor <= UPPER_BOUND)
      {
        return this->_capacity;
      }

      return this->_capacity * 2;
    }

    int optimal_capacity_down (long M) const
    {
      if (M == 0)
      {
        return 1;
      }

      int cap = 1;
      double factor = ((double) M / cap);

      while (factor >= LOWER_BOUND)
      {
        cap = cap * 2;
        factor = ((double) M / cap);
      }

      return cap / 2;
    }

    /**
     * Hashes the given key into an integer from 0 to (capacity-1).
     * @param key key to hash
     * @return hashed key
     */
    int hash (const KeyT &key) const
    {
      return this->_hash_func (key) & (this->_capacity - 1);
    }
};

template <class KeyT, class ValueT>
class HashMap<KeyT, ValueT>::ConstIterator
{
  public:
    typedef entry value_type;
    typedef const entry &reference;
    typedef const entry *pointer;
    typedef std::ptrdiff_t difference_type;
    typedef std::forward_iterator_tag iterator_category;

    ConstIterator (const HashMap<KeyT, ValueT> *hash_map) : _map (hash_map)
    {
      this->set_to_available_bucket (0);
    }

    ConstIterator (const HashMap<KeyT, ValueT> *hash_map, int bucket_index,
                   int entry_index)
        : _map (hash_map)
    {
      this->bucket_index = bucket_index;
      this->entry_index = entry_index;
    }

    reference operator* () const
    {
      // TODO: ask ofek if we need this code to prevent segfaults
      // if (this->bucket_index == -1 && this->entry_index == -1)
      // {
      //   return nullptr;
      // }

      return (*this->_map)._data[this->bucket_index][this->entry_index];
    }

    pointer operator->() const
    {
      if (this->bucket_index == -1 && this->entry_index == -1)
      {
        return nullptr;
      }

      return &(*this->_map)._data[this->bucket_index][this->entry_index];
    }

    ConstIterator &operator++ ()
    {
      this->entry_index++;

      if ((unsigned long) (*this->_map)._data[this->bucket_index].size ()
          == (unsigned long) this->entry_index)
      {
        this->set_to_available_bucket (this->bucket_index + 1);
      }

      return *this;
    }

    ConstIterator operator++ (int)
    {
      ConstIterator tmp = *this;
      ++(*this);
      return tmp;
    }

    friend bool operator== (const ConstIterator &a, const ConstIterator &b)
    {
      return a._map == b._map && a.bucket_index == b.bucket_index
             && a.entry_index == b.entry_index;
    };

    friend bool operator!= (const ConstIterator &a, const ConstIterator &b)
    {
      return !(a == b);
    };

  private:
    const HashMap<KeyT, ValueT> *_map;
    int bucket_index, entry_index;

    /**
     * A util function to find the first bucket starting from ${starting_pos}
     * that is not empty.
     * Once such bucket found, it sets bucket_index to that bucket index and
     * entry_index to 0.
     * If no bucket was found we set it to -1 to represent the end of the
     * iterator
     *
     * @param starting_pos Minimum bucket index
     */
    void set_to_available_bucket (int starting_pos)
    {
      for (int i = starting_pos; i < (*this->_map)._capacity; i++)
      {
        bucket &buck = (*this->_map)._data[i];
        if (!buck.empty ())
        {
          this->bucket_index = i;
          this->entry_index = 0;
          return;
        }
      }

      this->bucket_index = ITERATOR_END_VALUE;
      this->entry_index = ITERATOR_END_VALUE;
    }
};

#endif