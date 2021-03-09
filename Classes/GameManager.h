#pragma once

class GameData;

class GameManager
{
 //public:
 //    static GameManager& getInstance()
 //    {
 //        static GameManager    instance; // Guaranteed to be destroyed.
 //                              // Instantiated on first use.
 //        return instance;
 //    }
 //   private:
 //       GameManager() {}                    // Constructor? (the {} brackets) are needed here.

 //       // C++ 03
 //       // ========
 //       // Don't forget to declare these two. You want to make sure they
 //       // are inaccessible(especially from outside), otherwise, you may accidentally get copies of
 //       // your singleton appearing.
 //       GameManager(GameManager const&);              // Don't Implement
 //       void operator=(GameManager const&); // Don't implement

 //       // C++ 11
 //       // =======
 //       // We can use the better technique of deleting the methods
 //       // we don't want.
 //   public:
 //       GameManager(GameManager const&) = delete;
 //       void operator=(GameManager const&) = delete;

 //       // Note: Scott Meyers mentions in his Effective Modern
 //       //       C++ book, that deleted functions should generally
 //       //       be public as it results in better error messages
 //       //       due to the compilers behavior to check accessibility
 //       //       before deleted status


};
