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
#' @return A list with a 2D array of pixels and the parameters as a JSON.
#' @examples
#' library(IFSPlot)
#'
#' m1 = matrix(data = c(+0.00, +0.00, +0.00, +0.16), nrow = 2, ncol = 2)
#' m2 = matrix(data = c(+0.85, -0.04, +0.04, +0.85), nrow = 2, ncol = 2)
#' m3 = matrix(data = c(+0.20, +0.23, -0.26, +0.22), nrow = 2, ncol = 2)
#' m4 = matrix(data = c(-0.15, +0.26, +0.28, +0.24), nrow = 2, ncol = 2)
#'
#' v1 = c(0.00, 0.00)
#' v2 = c(0.00, 1.60)
#' v3 = c(0.00, 1.60)
#' v4 = c(0.00, 0.44)
#'
#' probability = c(1, 85, 7, 7)
#'
#' transformations = list(m1, m2, m3, m4)
#' translations    = list(v1, v2, v3, v4)
#'
#' iterations = 10**4
#'
#' result = RunIFS2D(transformations, translations, probability, iterations, 600*1200, "out")
#' @export
RunIFS2D = function(transformation, translation, probability, iterations, pixels, set_palette = 0, filename = "") {
  result = IFS2D(transformation, translation, probability, iterations, pixels)
  width  = dim(result$ImageMatrix)[1]
  height = dim(result$ImageMatrix)[2]
  par(mar=c(0, 0, 0, 0))
  if(set_palette == 0) palette("default")
  if(set_palette == 1) palette(gray(length(table(result$ImageMatrix)):0 / length(table(result$ImageMatrix))))
  if(set_palette == 2) palette(rev(rich.colors(length(table(result$ImageMatrix)))))
  if(set_palette == 0) {
    result$ImageMatrix[result$ImageMatrix > 1] = 1
    if(filename != "") {
      png(filename = filename, width = width, height = height)
      image(result$ImageMatrix, useRaster=TRUE, axes=FALSE, col = 0:1)
      dev.off()
    } else image(result$ImageMatrix, useRaster=TRUE, axes=FALSE, col = 0:1)
  }
  else
    if(filename != "") {
      png(filename = filename, width = width, height = height)
      image(result$ImageMatrix, useRaster=TRUE, axes=FALSE, col = 0:length(table(result$ImageMatrix)))
      dev.off()
    } else image(result$ImageMatrix, useRaster=TRUE, axes=FALSE, col = 0:length(table(result$ImageMatrix)))

  parameters = list()
  parameters$transformation = transformation
  parameters$translation    = translation
  parameters$probability    = probability
  parameters$iterations     = iterations
  parameters$pixels         = pixels

  result$JSON = toJSON(parameters)

  result
}

#' RunJSONIFS2D
#'
#' This function allows you to plot some fractals using the Iterated Function Systems theory.
#' @param JSON           A JSON string, URL or file.
#' @param set_palette    0 = default palette; 1 = grayscale; 2 = rich.colors.
#' @param filename       If given, saves the image as a png file in the current working directory.
#' @return A list with a 2D array of pixels and the parameters as a JSON.
#' @examples
#' library(IFSPlot)
#'
#' file_ifs = system.file("extdata", "Fern2D.json", package="IFSPlot")
#' result = RunJSONIFS2D(file_ifs, 1)
#' @export
RunJSONIFS2D = function(JSON, set_palette = 0, filename = "") {
  parameters = jsonlite::fromJSON(JSON, simplifyVector = F)
  transformations = lapply(parameters$transformation, function(x) matrix(data = unlist(x), nrow = length(x), ncol = length(x), byrow = T) )
  translations    = lapply(parameters$translation, function(x) { unlist(x) })
  probability     = unlist(parameters$probability)
  iterations      = unlist(parameters$iterations)
  pixels          = unlist(parameters$pixels)

  result = RunIFS2D(transformations, translations, probability, iterations, pixels, set_palette, filename)

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
#' @return A list with a 3D array of pixels and the parameters as a JSON.
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

  parameters = list()
  parameters$transformation = transformation
  parameters$translation    = translation
  parameters$probability    = probability
  parameters$iterations     = iterations
  parameters$pixels         = pixels
  parameters$width          = width
  parameters$height         = height

  result$JSON = toJSON(parameters)

  result
}

#' RunJSONIFS3D
#'
#' This function allows you to plot some fractals using the Iterated Function Systems theory.
#' @param JSON           A JSON string, URL or file.
#' @param width          Width of the window.
#' @param height         Height of the window.
#' @param set_palette    0 = default palette; 1 = grayscale; 2 = rich.colors.
#' @return A list with a 3D array of pixels and the parameters as a JSON.
#' @export
RunJSONIFS3D = function(JSON, width, height, set_palette = 0) {
  parameters = jsonlite::fromJSON(JSON, simplifyVector = F)
  transformations = lapply(parameters$transformation, function(x) matrix(data = unlist(x), nrow = length(x), ncol = length(x), byrow = T) )
  translations    = lapply(parameters$translation, function(x) { unlist(x) })
  probability     = unlist(parameters$probability)
  iterations      = unlist(parameters$iterations)
  pixels          = unlist(parameters$pixels)
  width           = unlist(parameters$width)
  height          = unlist(parameters$height)

  result = RunIFS3D(transformations, translations, probability, iterations, pixels, width, height, set_palette)

  result
}
