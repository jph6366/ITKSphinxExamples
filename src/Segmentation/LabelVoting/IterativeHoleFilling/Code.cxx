/*=========================================================================
 *
 *  Copyright NumFOCUS
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         https://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkVotingBinaryIterativeHoleFillingImageFilter.h"

int
main(int argc, char * argv[])
{
  if (argc != 6)
  {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0];
    std::cerr << " <InputFileName> <OutputFileName> <radius> <majority threshold> <number of iterations>";
    std::cerr << std::endl;
    return EXIT_FAILURE;
  }

  const char * inputFileName = argv[1];
  const char * outputFileName = argv[2];

  int          r = std::stoi(argv[3]);
  int          majorityThreshold = std::stoi(argv[4]);
  unsigned int numberOfIterations = std::stoi(argv[5]);

  constexpr unsigned int Dimension = 2;

  using PixelType = unsigned char;
  using ImageType = itk::Image<PixelType, Dimension>;

  const auto input = itk::ReadImage<ImageType>(inputFileName);

  using FilterType = itk::VotingBinaryIterativeHoleFillingImageFilter<ImageType>;
  auto radius = FilterType::InputSizeType::Filled(r);

  auto filter = FilterType::New();
  filter->SetInput(input);
  filter->SetRadius(radius);
  filter->SetMajorityThreshold(majorityThreshold);
  filter->SetBackgroundValue(itk::NumericTraits<PixelType>::Zero);
  filter->SetForegroundValue(itk::NumericTraits<PixelType>::max());
  filter->SetMaximumNumberOfIterations(numberOfIterations);

  try
  {
    itk::WriteImage(filter->GetOutput(), outputFileName);
  }
  catch (const itk::ExceptionObject & error)
  {
    std::cerr << "Error: " << error << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
