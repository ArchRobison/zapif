#if defined(A)||zero/* trailing stuff */ 
Line above should become #ifdef A
#endif

#if defined B||zero/* trailing stuff */
Line above should become #ifdef B
#endif

#if zero||!defined C/* trailing stuff */
Line above should become #ifndef C
#endif

#if zero||!defined D/* trailing stuff */
Line above should become #ifndef D
#endif

#if(defined E||zero)/* trailing stuff */
Line above should become #ifdef E, not #ifdefE 
#endif

#if(!defined F||zero)/* trailing stuff */
Line above should become #ifndef F, not #ifdefF
#endif

#if zero
#elif defined(G)
Line above should become #ifdef G
#else
Whatever
#endif

#if zero
#elif !defined(H)
Line above should become #ifndef H
#else
More text
#endif

#if !defined(nil)&&!defined(I) /*trailing stuff */
Line above should become #ifndef I
#endif
