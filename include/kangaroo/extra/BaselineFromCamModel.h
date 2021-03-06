#pragma once

#include <calibu/Calibu.h>

inline Sophus::SE3d T_rlFromCamModelRDF(
    const std::shared_ptr<calibu::CameraInterface<double>> lcmod,
    const std::shared_ptr<calibu::CameraInterface<double>> rcmod,
    const Eigen::Matrix3d& targetRDF)
{
    // Transformation matrix to adjust to target RDF
    Eigen::Matrix4d Tadj[2] = {Eigen::Matrix4d::Identity(),Eigen::Matrix4d::Identity()};
    Tadj[0].block<3,3>(0,0) = targetRDF.transpose() * lcmod->RDF();
    Tadj[1].block<3,3>(0,0) = targetRDF.transpose() * rcmod->RDF();

    // Computer Poses in our adjust coordinate system
    const Eigen::Matrix4d T_lw_ = Tadj[0] * lcmod->Pose().matrix().inverse();
    const Eigen::Matrix4d T_rw_ = Tadj[1] * rcmod->Pose().matrix().inverse();

    // Computer transformation to right camera frame from left
    const Eigen::Matrix4d T_rl = T_rw_ * T_lw_.inverse();

    return Sophus::SE3d(T_rl.block<3,3>(0,0), T_rl.block<3,1>(0,3) );
}
