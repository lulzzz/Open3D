// ----------------------------------------------------------------------------
// -                        Open3D: www.open-3d.org                            -
// ----------------------------------------------------------------------------
// Imported on Apr 11th, 2018 from the Open3D project
// Copyright (c) 2018 www.open3d.org
// released under MIT licence
//
// A fork of their project is avilable at legacy-code.open-3d.org
// Refer to http://legacy-code.open-3d.org/blob/master/LICENSE
// for their full licence
// Check http://legacy-code.open-3d.org/graphs/contributors
// to get the list of their contributors and their contributions
//
// The rights of any later updates or modifications are reserved
// ----------------------------------------------------------------------------
// The MIT License (MIT)
//
// Copyright (c) 2018, Hamdi Sahloul - www.open-3d.org
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
// ----------------------------------------------------------------------------

#pragma once

#include <Eigen/Core>
#include <Open3D/Core/Utility/IJsonConvertible.h>

namespace open3d {

class ViewParameters : public IJsonConvertible
{
public:
    typedef Eigen::Matrix<double, 17, 4, Eigen::RowMajor> Matrix17x4d;
    typedef Eigen::Matrix<double, 17, 1> Vector17d;

public:
    ViewParameters() {}
    ~ViewParameters() override {}

public:
    Vector17d ConvertToVector17d();
    void ConvertFromVector17d(const Vector17d &v);
    bool ConvertToJsonValue(Json::Value &value) const override;
    bool ConvertFromJsonValue(const Json::Value &value) override;

public:
    double field_of_view_;
    double zoom_;
    Eigen::Vector3d lookat_;
    Eigen::Vector3d up_;
    Eigen::Vector3d front_;
    Eigen::Vector3d boundingbox_min_;
    Eigen::Vector3d boundingbox_max_;
};

}   // namespace open3d
