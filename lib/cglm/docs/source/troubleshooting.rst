.. default-domain:: C

Troubleshooting
================================================================================

It is possible that sometimes you may get crashes or wrong results.
Follow these topics

Memory Allocation:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Again, **cglm** doesn't alloc any memory on heap.
cglm functions works like memcpy; it copies data from src,
makes calculations then copy the result to dest.

You are responsible for allocation of **src** and **dest** parameters.

Alignment:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**vec4** and **mat4** types requires 16 byte alignment.
These types are marked with align attribute to let compiler know about this
requirement.

But since MSVC (Windows) throws the error:

**"formal parameter with requested alignment of 16 won't be aligned"**

The alignment attribute has been commented for MSVC

.. code-block:: c

   #if defined(_MSC_VER)
   #  define CGLM_ALIGN(X) /* __declspec(align(X)) */
   #else
   #  define CGLM_ALIGN(X) __attribute((aligned(X)))
   #endif.

So MSVC may not know about alignment requirements when creating variables.
The interesting thing is that, if I remember correctly Visual Studio 2017
doesn't throw the above error. So we may uncomment that line for Visual Studio 2017,
you may do it yourself.

**This MSVC issue is still in TODOs.**

**UPDATE:** By starting v0.4.5 cglm provides an option to disable alignment requirement.
Also alignment is disabled for older msvc verisons as default. Now alignment is only required in Visual Studio 2017 version 15.6+ if CGLM_ALL_UNALIGNED macro is not defined.

Crashes, Invalid Memory Access:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Probably you are trying to write to invalid memory location.

You may used wrong function for what you want to do.

For instance you may called **glm_vec4_** functions for **vec3** data type.
It will try to write 32 byte but since **vec3** is 24 byte it should throw
memory access error or exit the app without saying anything.

**UPDATE - IMPORTANT:** 

  | On MSVC or some other compilers, if alignment is enabled (default) then double check alignment requirements if you got a crash.

  | If you send GLM_VEC4_ONE or similar macros directly to a function, it may be crashed.
  | Because compiler may not apply alignment as defined on **typedef** to that macro while passing it (on stack) to a function.

Wrong Results:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Again, you may used wrong function.

For instance if you use **glm_normalize()** or **glm_vec3_normalize()** for **vec4**,
it will assume that passed param is **vec3** and will normalize it for **vec3**.
Since you need to **vec4** to be normalized in your case, you will get wrong results.

Accessing vec4 type with vec3 functions is valid, you will not get any error, exception or crash.
You only get wrong results if you don't know what you are doing!

So be carefull, when your IDE (Xcode, Visual Studio ...) tried to autocomplete function names, READ IT :)

**Also implementation may be wrong please let us know by creating an issue on Github.**

BAD_ACCESS : Thread 1: EXC_BAD_ACCESS (code=EXC_I386_GPFLT) or Similar Errors/Crashes
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This is similar issue with alignment. For instance if you compiled **cglm** with 
AVX (**-mavx**, intentionally or not) and if you use **cglm** in an environment that doesn't 
support AVX (or if AVX is disabled intentionally) e.g. environment that max support SSE2/3/4, 
then you probably get **BAD ACCESS** or similar...

Because if you compile **cglm** with AVX it aligns **mat4** with 32 byte boundary, 
and your project aligns that as 16 byte boundary...

Check alignment, supported vector extension or simd in **cglm** and linked projects...

Other Issues?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**Please let us know by creating an issue on Github.**
