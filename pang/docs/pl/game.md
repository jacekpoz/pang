# Game

Tak, zgadłeś - w dzisiejszym odcinku Dokumentacji będziemy mówić o klasie `Game`. Kto by pomyślał.

Myślę że taki sposób na napisanie głównej klasy jest dosyć standardowy, przynajmniej w mniejszych grach. Pojedyncza klasa `Game`, posiadająca większość stanu gry i działająca jako pętla gry. Niektóre funkcje są identyczne do tych w `System`ach, głównie chodzi tu o `update()`, `render()`, `pause()` i `resume()`, które wywołują funkcję `update()` poszczególnie w każdym z `System`ów i każdym z `RenderingSystem`ów i `pause()` i `resume()` w każdym z `System`ów.

`Game` posiada również `sf::Font` wykorzystywany w całej grze i zmienne wykorzystywane w całej grze, zawierające między innymi przełączniki do trybu debugowania, do pauzowania i skupiania (chodzi tu o pauzowanie gry, gdy wybieramy inne okno).

Wraz z grą, `Game` musi również rosnąć, ale w przeciwieństwie do `System`ów, nie uważam tego za duży problem. W końcu jest tylko jedno `Game`, ale wiele `System`ów.
