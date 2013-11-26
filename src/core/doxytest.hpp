/*!
 * This should be a file-wide comment
 */

namespace Core
{
    /*! 
       This is a brief explanation that ends at the punctuation.
       This is part of the paragraph that details the fuction more
      
       \param i "i" is a parameter and this is its information
       \return this is a description of what the functions returns
     */
    int MyFunction( int i );

    /*!
       This class is supposedly hilarious.
     */
    class LOL
    {
    public:
        /*!
           You might never *get* this.
         */
        char* GetSimpleJokePointer();
    private:

        /*!
           This is a private joke, and will not be exported to the doxygen
           Making this comment completely redundant!
         */
        std::string GetPrivateJoke();
    };
}
