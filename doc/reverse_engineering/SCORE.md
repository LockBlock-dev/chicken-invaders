# Score system

The game awards points for various in-game actions. Below are the scoring rules and their corresponding formulas:

## Asteroid destroyed

Formula: `100 * (velocityX + velocityY) + 300`

Points are awarded based on the asteroid's horizontal and vertical velocity, scaled by 100, with an additional base value of 300 points.

## Boss hit

Formula: `250`

Each successful hit on a boss gives 250 points.

## Boss killed

Formula: `1000 * currentSystem + 10000`

Points are awarded based on the current star system, multiplied by 1000, with an additional base value of 10000 points.

## Chicken killed

Formula: `100 * currentSystem + 1000`

Points are awarded based on the current star system, multiplied by 100, with an additional base value of 1000 points.

## Chicken leg collected

Formula: `200`

Each collected chicken leg gives 200 points.
