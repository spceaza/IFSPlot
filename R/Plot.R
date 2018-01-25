#' RunChaosGame
#'
#' This function allows you to plot some fractals using the Iterated Function Systems theory.
#' @param transformation A list of size n, the 2 x 2 transformation matrices.
#' @param translation    A list of size n, the 2 x 2 translational vectors.
#' @param probability    A list of size n, the weighted probabilities.
#' @param iterations     The number of iterations/points.
#' @param width          The width of the image.
#' @param height         The height of the image.
#' @param set_palette    0 = default palette; 1 = grayscale; 2 = rich.colors.
#' @return A matrix of black/white pixels.
#' @export
#' @examples
#' pixel_matrix = IFSPlot::RunChaosGame(5, .4, 100**3)
RunIFS = function(transformation, translation, probability, iterations, width, height, set_palette = 0) {
  result = IFS(transformation, translation, probability, iterations, width, height)
  par(mar=c(0, 0, 0, 0))
  if(set_palette == 0) palette("default")
  if(set_palette == 1) palette(gray(length(table(result)):0 / length(table(result))))
  if(set_palette == 2) palette(rev(rich.colors(length(table(result)))))
  if(set_palette == 0) {
    result[result > 1] = 1
    image(result, useRaster=TRUE, axes=FALSE, col = 0:1)
  }
  else
    image(result, useRaster=TRUE, axes=FALSE, col = 0:length(table(result)))
    
  result
}
