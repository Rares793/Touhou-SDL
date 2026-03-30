#Working with git stuff:
git checkout -b feature/player-focus

# code stuff...

git add .
git commit -m "add focus movement"

git push -u origin feature/player-focus

# merge
git checkout main
git pull
git merge feature/player-focus
git push

# cleanup
git branch -d feature/player-focus
git push origin --delete feature/player-focus

# compiling
g++ main.cpp sources/*.cpp -Iinclude $(pkg-config --cflags --libs sdl3 sdl3-image) -o game
