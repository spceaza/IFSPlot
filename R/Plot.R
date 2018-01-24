#' RunChaosGame
#'
#' This function allows you to plot some fractals using the Iterated Function Systems theory.
#' @param transformation A list of size n, the 2 x 2 transformation matrices.
#' @param translation    A list of size n, the 2 x 2 translational vectors.
#' @param probability    A list of size n, the weighted probabilities.
#' @param iterations     The number of iterations/points.
#' @return A matrix of black/white pixels.
#' @export
#' @examples
#' pixel_matrix = IFSPlot::RunChaosGame(5, .4, 100**3)
RunIFS = function(transformation, translation, probability, iterations, x_res, y_res) {
  result = IFS(transformation, translation, probability, iterations, x_res, y_res)
  par(mar=c(0, 0, 0, 0))
  image(result, useRaster=TRUE, axes=FALSE, col = c(0, 1))
  result
}
