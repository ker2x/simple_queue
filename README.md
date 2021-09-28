# simple_queue

## Project

a tentative to remake this without copypasta :
https://github.com/Deaod/spsc_queue

## File

* spsc_queue.h : a cleaned version of the original code that i keep here for benchmark reference
* simple_queue.h : my code
* main.cpp : the benchmarking code


## Interface
### Summary
There are a lot of different ways to enqueue and dequeue elements.
* `write` enqueues from iterators
* `push` and `push_n` enqueue from a reference
* `emplace` and `emplace_n` enqueue by constructing elements from the provided arguments in-place
* `produce` and `produce_n` enqueue by calling the provided callable, which is expected to construct an element using placement new
* `read` dequeues by moving to iterators
* `pop` dequeues by moving to a reference
* `consume` and `consume_all` dequeue by calling the provided callable for each element
* `discard` dequeues the next element without any way of looking at it
* `front` returns a pointer to the next element


### Template
spsc_queue takes up to three template parameters:
* `T`: The type of a single element
* `queue_size`: The number of slots for elements within the queue.  
  Note: Due to implementation details, one slot is reserved and
  cannot be used.
* `align_log2`: The number of bytes to align on, expressed as an exponent for
  two, so the actual alignment is `(1 << align_log2)` bytes. This
  number should be at least `log2(alignof(size_t))`. Ideal values
  avoid destructive hardware interference (false sharing).  
  Default is 7.  
  `alignof(T)` must not be greater than `(1 << align_log2)`.

### General
```c++
bool is_empty() const;
```
Returns `true` if there is currently no object in the queue. Returns `false` otherwise.

```c++
bool is_full() const;
```
Returns `true` if no more objects can be added to the queue. Returns `false` otherwise.

### Enqueue
```c++
bool push(const T& elem);
bool push(T&& elem);
```
Tries to insert `elem` into the queue. Returns `true` if successful, `false` otherwise.

```c++
size_type push_n(size_type count, const T& elem);
```
Tries to insert `count` copies of `elem` into the queue. Returns the number of copies successfully inserted.

```c++
template<typename Iterator>
size_type write(Iterator beg, Iterator end);
```
Tries to copy elements into the queue from `beg`, until `end` is reached. Returns the number of elements copied into the queue.

```c++
template<typename Iterator>
size_type write(size_type count, Iterator elems);
```
Tries to copy `count` elements into the queue from `elems` until either the queue is full or all have been copied. Returns the number of elements copied into the queue.

```c++
template<typename... Args>
bool emplace(Args&&... args);
```
Tries to insert an object of type `T` constructed from `args` into the queue. Returns `true` if successful, `false` otherwise.

```c++
template<typename... Args>
size_type emplace_n(size_type count, Args&&... args);
```
Tries to insert `count` objects of type `T` constructed from `args` into the queue. Returns the number of objects successfully inserted.

```c++
template<typename Callback>
bool produce(Callback&& cb);
```
Tries to insert an object into the queue by calling `Callback` if there is space for an object. Returns `true` if there was space for an object, and `Callback` returned `true`. Returns `false` otherwise.  
`Callback` is an invocable with one parameter of type `void*`, and a return type of `bool`. Callback is expected to place a new object of type `T` at the address passed to it.

```c++
template<typename Callback>
size_type produce_n(size_type count, Callback&& cb);
```
Tries to insert `count` objects into the queue by calling `Callback` as long as there is space in the queue, or until Callback returns `false` once.  
Returns the number of times `Callback` was invoked and returned `true`.  
`Callback` is an invocable with one parameter of type `void*`, and a return type of `bool`. `Callback` is expected to place a new object of type `T` at the address passed to it.

### Dequeue
```c++
const T* front() const;
T* front();
```
Returns a pointer to the next object in the queue, if such an object exists. Returns `nullptr` if the queue is empty.

```c++
void discard();
```
Removes the next object from the queue. This function must not be called if the queue is empty.

```c++
bool pop(T& out);
```
Tries to move the next object in the queue into `out`, if such an object exists. Returns `true` if out contains a new object. Returns `false` if the queue was empty.

```c++
template<typename Iterator>
size_type read(Iterator beg, Iterator end)
```
Tries to move elements out of the queue to `[beg .. end)`, until either all have been moved or the queue is empty. Returns the number of elements that were moved.

```c++
template<typename Iterator>
size_type read(size_type count, Iterator elems)
```
Tries to move elements out of the queue to `[elems .. elems + count)`, until either `count` elements have been moved, or the queue is empty. Returns the number of elements that were moved.

```c++
template<typename Callback>
bool consume(Callback&& cb);
```
Tries to remove an object from the queue by calling `Callback` and passing the object to it. Returns `true` if there was an object in the queue and `Callback` returned `true`. Returns `false` otherwise.  
`Callback` is an invocable with one parameter of type `T*`, and a return type of `bool`.

```c++
template<typename Callback>
size_type consume_all(Callback&& cb);
```
Tries to remove all objects from the queue by calling `Callback` for each object, passing the address of each object to it, until either the queue is empty, or Callback returns `false`. Returns the number of times `Callback` was invoked and returned `true`.  
`Callback` is an invocable with one parameter of type `T*`, and a return type of `bool`.

### Exception Safety
All functions provide at least the basic exception safety guarantee. In special circumstances, like when using `write` with contiguous or random access iterators and `T` being trivially copy-constructible, a strong exception safety is guaranteed.

Generally, an enqueue or dequeue operation will try to commit as much progress as possible in case of a thrown exception. This avoids having to undo uncommitted changes during stack unwinding.

### Parameters
The test was run on a laptop running Windows 10 1903, with an i7-6600U running at 3.1 to 3.2GHz, and 8GB of DDR4-2133 RAM. The test program was compiled using MSVC 19.22. Producer and consumer thread were pinned to different physical cores.