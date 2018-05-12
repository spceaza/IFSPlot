# IFSPlot [![Build Status](https://travis-ci.org/cesandovalp/IFSPlot.svg)](https://travis-ci.org/cesandovalp/IFSPlot) [![License](https://cesandovalp.github.io/assets/images/GPL2+.svg)](http://www.gnu.org/licenses/gpl-2.0.html) [![CRAN](http://www.r-pkg.org/badges/version/IFSPlot)](https://cran.r-project.org/package=IFSPlot) [![Downloads](http://cranlogs.r-pkg.org/badges/IFSPlot?color=brightgreen)](http://www.r-pkg.org/pkg/IFSPlot)

Plot fractals using the Iterated Function Systems in C++.
Fractals such as clouds or plants can be generated using the iteration of one or more affine transformations.

# Installation

```R
# install.packages("devtools")
devtools::install_github("cesandovalp/IFSPlot")
```

# Examples

## Barnsley Fern

### Code

```R
library(IFSPlot)

m1 = matrix(data = c(+0.00, +0.00, +0.00, +0.16), nrow = 2, ncol = 2)
m2 = matrix(data = c(+0.85, -0.04, +0.04, +0.85), nrow = 2, ncol = 2)
m3 = matrix(data = c(+0.20, +0.23, -0.26, +0.22), nrow = 2, ncol = 2)
m4 = matrix(data = c(-0.15, +0.26, +0.28, +0.24), nrow = 2, ncol = 2)

v1 = c(0.00, 0.00)
v2 = c(0.00, 1.60)
v3 = c(0.00, 1.60)
v4 = c(0.00, 0.44)

probability = c(1, 85, 7, 7)

transformations = list(m1, m2, m3, m4)
translations    = list(v1, v2, v3, v4)

iterations = 10**7

result = RunIFS2D(transformations, translations, probability, iterations, 600*1200, 1)
```
### Output

<img src="docs/fern_grayscale.png" width="200">

## More Examples

[More examples](Examples.md)

## Status

## Author

Cristian Sandoval-Pineda

## License

GPL (>= 2)
