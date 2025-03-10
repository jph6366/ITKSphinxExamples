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
#include <itkImage.h>
#include <itkMetaDataDictionary.h>
#include <itkMetaDataObject.h>
#include <itkImageRegionIterator.h>
#include <itkImageFileWriter.h>
#include <itkImageFileReader.h>

using ImageType = itk::Image<unsigned char, 2>;

static void
CreateImage(ImageType::Pointer image);

int
main()
{
  // Create an image
  auto image = ImageType::New();
  CreateImage(image);

  // Store some data in it
  itk::MetaDataDictionary dictionary;
  itk::EncapsulateMetaData<float>(dictionary, "ASimpleFloat", 1.2);
  image->SetMetaDataDictionary(dictionary);

  // View all of the data
  dictionary.Print(std::cout);

  // View the data individually
  auto itr = dictionary.Begin();

  while (itr != dictionary.End())
  {
    std::cout << "Key   = " << itr->first << std::endl;
    std::cout << "Value = ";
    itr->second->Print(std::cout);
    std::cout << std::endl;
    ++itr;
  }

  // Write the image (and the data) to a file
  using WriterType = itk::ImageFileWriter<ImageType>;
  auto writer = WriterType::New();
  writer->SetFileName("test.mhd");
  writer->SetInput(image);
  writer->Update();

  // Read the image (and data) from the file
  using ReaderType = itk::ImageFileReader<ImageType>;
  auto reader = ReaderType::New();
  reader->SetFileName("test.mhd");

  // Display the data
  std::cout << "Data read from file:" << std::endl;
  reader->GetMetaDataDictionary().Print(std::cout);

  return EXIT_SUCCESS;
}

void
CreateImage(ImageType::Pointer image)
{
  ImageType::IndexType start{};

  auto size = ImageType::SizeType::Filled(10);

  ImageType::RegionType region;
  region.SetSize(size);
  region.SetIndex(start);

  image->SetRegions(region);
  image->Allocate();

  itk::ImageRegionIterator<ImageType> imageIterator(image, image->GetLargestPossibleRegion());

  while (!imageIterator.IsAtEnd())
  {
    imageIterator.Set(20);
    ++imageIterator;
  }
}
