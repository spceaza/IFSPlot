#' RunIFS2D
#'
#' This function allows you to plot some fractals using the Iterated Function Systems theory.
#' @param transformation A list of size n, the 2 x 2 transformation matrices.
#' @param translation    A list of size n, the 2 x 1 translational vectors.
#' @param probability    A list of size n, the weighted probabilities.
#' @param iterations     The number of iterations/points.
#' @param pixels         The number of pixels for the 2D plot.
#' @param set_palette    0 = default palette; 1 = grayscale; 2 = rich.colors.
#' @param filename       If given, saves the image as a png file in the current working directory.
#' @return A 2D array of pixels.
#' @export
RunIFS2D = function(transformation, translation, probability, iterations, pixels, set_palette = 0, filename = "") {
  result = IFS2D(transformation, translation, probability, iterations, pixels)
  par(mar=c(0, 0, 0, 0))
  if(set_palette == 0) palette("default")
  if(set_palette == 1) palette(gray(length(table(result$ImageMatrix)):0 / length(table(result$ImageMatrix))))
  if(set_palette == 2) palette(rev(rich.colors(length(table(result$ImageMatrix)))))
  if(set_palette == 0) {
    result$ImageMatrix[result$ImageMatrix > 1] = 1
    if(filename != "") {
      png(filename = filename, width = result$width, height = result$height)
      image(result$ImageMatrix, useRaster=TRUE, axes=FALSE, col = 0:1)
      dev.off()
    }
  }
  else
    if(filename != "") {
      png(filename = filename, width = result$width, height = result$height)
      image(result$ImageMatrix, useRaster=TRUE, axes=FALSE, col = 0:length(table(result$ImageMatrix)))
      dev.off()
    }

  result
}

#' RunIFS3D
#'
#' This function allows you to plot some fractals using the Iterated Function Systems theory.
#' @param transformation A list of size n, the 3 x 3 transformation matrices.
#' @param translation    A list of size n, the 3 x 1 translational vectors.
#' @param probability    A list of size n, the weighted probabilities.
#' @param iterations     The number of iterations/points.
#' @param pixels         The number of pixels for the 3D plot.
#' @param width          Width of the window.
#' @param height         Height of the window.
#' @param set_palette    0 = default palette; 1 = grayscale; 2 = rich.colors.
#' @return A 3D array of pixels.
#' @export
RunIFS3D = function(transformation, translation, probability, iterations, pixels, width, height, set_palette = 0) {
  result = IFS3D(transformation, translation, probability, iterations, pixels)
  xyz = which(result$ImageMatrix > 0, arr.ind=TRUE)

  if(set_palette == 0) {
    df = data.frame(x = xyz[, 1], y = xyz[, 2], z = xyz[, 3])
    palette("default")
    with(df, plot3d(x, y, z))
  }
  if(set_palette == 1) {
    df = data.frame(x = xyz[,1], y = xyz[,2], z = xyz[,3], colorby = result$ImageMatrix[xyz])
    palette(gray(length(table(result$ImageMatrix[xyz])):0 / length(table(result$ImageMatrix[xyz]))))
    with(df, plot3d(x, y, z, col = colorby))
  }
  if(set_palette == 2) {
    df = data.frame(x=xyz[,1], y=xyz[,2], z=xyz[,3], colorby=result$ImageMatrix[xyz])
    palette(rev(rich.colors(length(table(result$ImageMatrix[xyz])))))
    with(df, plot3d(x, y, z, col = colorby))
  }

  par3d(windowRect = c(0, 0, width, height))

  result
}
