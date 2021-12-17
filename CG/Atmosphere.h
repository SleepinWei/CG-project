/**
 * Copyright (c) 2017 Eric Bruneton
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holders nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

/*<h2>atmosphere/model.h</h2>

<p>This file defines the API to use our atmosphere model in OpenGL applications.
To use it:
<ul>
<li>create a <code>Model</code> instance with the desired atmosphere
parameters.</li>
<li>call <code>Init</code> to precompute the atmosphere textures,</li>
<li>link <code>GetShader</code> with your shaders that need access to the
atmosphere shading functions.</li>
<li>for each GLSL program linked with <code>GetShader</code>, call
<code>SetProgramUniforms</code> to bind the precomputed textures to this
program (usually at each frame).</li>
<li>delete your <code>Model</code> when you no longer need its shader and
precomputed textures (the destructor deletes these resources).</li>
</ul>

<p>The shader returned by <code>GetShader</code> provides the following
functions (that you need to forward declare in your own shaders to be able to
compile them separately):

<pre class="prettyprint">
// Returns the radiance of the Sun, outside the atmosphere.
vec3 GetSolarRadiance();

// Returns the sky radiance along the segment from 'camera' to the nearest
// atmosphere boundary in direction 'view_ray', as well as the transmittance
// along this segment.
vec3 GetSkyRadiance(vec3 camera, vec3 view_ray, double shadow_length,
    vec3 sun_direction, out vec3 transmittance);

// Returns the sky radiance along the segment from 'camera' to 'p', as well as
// the transmittance along this segment.
vec3 GetSkyRadianceToPoint(vec3 camera, vec3 p, double shadow_length,
    vec3 sun_direction, out vec3 transmittance);

// Returns the sun and sky irradiance received on a surface patch located at 'p'
// and whose normal vector is 'normal'.
vec3 GetSunAndSkyIrradiance(vec3 p, vec3 normal, vec3 sun_direction,
    out vec3 sky_irradiance);

// Returns the luminance of the Sun, outside the atmosphere.
vec3 GetSolarLuminance();

// Returns the sky luminance along the segment from 'camera' to the nearest
// atmosphere boundary in direction 'view_ray', as well as the transmittance
// along this segment.
vec3 GetSkyLuminance(vec3 camera, vec3 view_ray, double shadow_length,
    vec3 sun_direction, out vec3 transmittance);

// Returns the sky luminance along the segment from 'camera' to 'p', as well as
// the transmittance along this segment.
vec3 GetSkyLuminanceToPoint(vec3 camera, vec3 p, double shadow_length,
    vec3 sun_direction, out vec3 transmittance);

// Returns the sun and sky illuminance received on a surface patch located at
// 'p' and whose normal vector is 'normal'.
vec3 GetSunAndSkyIlluminance(vec3 p, vec3 normal, vec3 sun_direction,
    out vec3 sky_illuminance);
</pre>

<p>where
<ul>
<li><code>camera</code> and <code>p</code> must be expressed in a reference
frame where the planet center is at the origin, and measured in the unit passed
to the constructor's <code>length_unit_in_meters</code> argument.
<code>camera</code> can be in space, but <code>p</code> must be inside the
atmosphere,</li>
<li><code>view_ray</code>, <code>sun_direction</code> and <code>normal</code>
are unit direction vectors expressed in the same reference frame (with
<code>sun_direction</code> pointing <i>towards</i> the Sun),</li>
<li><code>shadow_length</code> is the length along the segment which is in
shadow, measured in the unit passed to the constructor's
<code>length_unit_in_meters</code> argument.</li>
</ul>

<p>and where
<ul>
<li>the first 4 functions return spectral radiance and irradiance values
(in $W.m^{-2}.sr^{-1}.nm^{-1}$ and $W.m^{-2}.nm^{-1}$), at the 3 wavelengths
<code>kLambdaR</code>, <code>kLambdaG</code>, <code>kLambdaB</code> (in this
order),</li>
<li>the other functions return luminance and illuminance values (in
$cd.m^{-2}$ and $lx$) in linear <a href="https://en.wikipedia.org/wiki/SRGB">
sRGB</a> space (i.e. before adjustements for gamma correction),</li>
<li>all the functions return the (unitless) transmittance of the atmosphere
along the specified segment at the 3 wavelengths <code>kLambdaR</code>,
<code>kLambdaG</code>, <code>kLambdaB</code> (in this order).</li>
</ul>

<p><b>Note</b> The precomputed atmosphere textures can store either irradiance
or illuminance values (see the <code>num_precomputed_wavelengths</code>
parameter):
<ul>
  <li>when using irradiance values, the RGB channels of these textures contain
  spectral irradiance values, in $W.m^{-2}.nm^{-1}$, at the 3 wavelengths
  <code>kLambdaR</code>, <code>kLambdaG</code>, <code>kLambdaB</code> (in this
  order). The API functions returning radiance values return these precomputed
  values (times the phase functions), while the API functions returning
  luminance values use the approximation described in
  <a href="https://arxiv.org/pdf/1612.04336.pdf">A Qualitative and Quantitative
  Evaluation of 8 Clear Sky Models</a>, section 14.3, to convert 3 radiance
  values to linear sRGB luminance values.</li>
  <li>when using illuminance values, the RGB channels of these textures contain
  illuminance values, in $lx$, in linear sRGB space. These illuminance values
  are precomputed as described in
  <a href="http://www.oskee.wz.cz/stranka/uploads/SCCG10ElekKmoch.pdf">Real-time
  Spectral Scattering in Large-scale Natural Participating Media</a>, section
  4.4 (i.e. <code>num_precomputed_wavelengths</code> irradiance values are
  precomputed, and then converted to sRGB via a numerical integration of this
  spectrum with the CIE color matching functions). The API functions returning
  luminance values return these precomputed values (times the phase functions),
  while <i>the API functions returning radiance values are not provided</i>.
  </li>
</ul>

<p>The concrete API definition is the following:
*/

#ifndef ATMOSPHERE_MODEL_H_
#define ATMOSPHERE_MODEL_H_

#include <glad/glad.h>
#include <array>
#include <functional>
#include <string>
#include <vector>
#include <memory>
#include"Camera.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glfw/glfw3.h>

extern Camera camera;
extern const unsigned int SCR_WDITH;
extern const unsigned int SCR_HEIGHT;


constexpr double kPi = 3.1415926;
constexpr double kSunAngularRadius = 0.00935 / 2.0;
constexpr double kSunSolidAngle = kPi * kSunAngularRadius * kSunAngularRadius;
constexpr double kLengthUnitInMeters = 1000.0;
const char kVertexShader[] = R"(
    #version 330
    uniform mat4 model_from_view;
    uniform mat4 view_from_clip;
    layout(location = 0) in vec4 vertex;
    out vec3 view_ray;
    void main() {
      view_ray =
          (model_from_view * vec4((view_from_clip * vertex).xyz, 0.0)).xyz;
      gl_Position = vertex;
    })";



namespace atmosphere {

    // An atmosphere layer of width 'width' (in m), and whose density is defined as
    //   'exp_term' * exp('exp_scale' * h) + 'linear_term' * h + 'constant_term',
    // clamped to [0,1], and where h is the altitude (in m). 'exp_term' and
    // 'constant_term' are unitless, while 'exp_scale' and 'linear_term' are in
    // m^-1.
    class DensityProfileLayer {
    public:
        DensityProfileLayer() : DensityProfileLayer(0.0, 0.0, 0.0, 0.0, 0.0) {}
        DensityProfileLayer(double width, double exp_term, double exp_scale,
            double linear_term, double constant_term)
            : width(width), exp_term(exp_term), exp_scale(exp_scale),
            linear_term(linear_term), constant_term(constant_term) {
        }
        double width;
        double exp_term;
        double exp_scale;
        double linear_term;
        double constant_term;
    };

    class Model {
    public:
        Model(
            // The wavelength values, in nanometers, and sorted in increasing order, for
            // which the solar_irradiance, rayleigh_scattering, mie_scattering,
            // mie_extinction and ground_albedo samples are provided. If your shaders
            // use luminance values (as opposed to radiance values, see above), use a
            // large number of wavelengths (e.g. between 15 and 50) to get accurate
            // results (this number of wavelengths has absolutely no impact on the
            // shader performance).
            const std::vector<double>& wavelengths,
            // The solar irradiance at the top of the atmosphere, in W/m^2/nm. This
            // vector must have the same size as the wavelengths parameter.
            const std::vector<double>& solar_irradiance,
            // The sun's angular radius, in radians. Warning: the implementation uses
            // approximations that are valid only if this value is smaller than 0.1.
            double sun_angular_radius,
            // The distance between the planet center and the bottom of the atmosphere,
            // in m.
            double bottom_radius,
            // The distance between the planet center and the top of the atmosphere,
            // in m.
            double top_radius,
            // The density profile of air molecules, i.e. a function from altitude to
            // dimensionless values between 0 (null density) and 1 (maximum density).
            // Layers must be sorted from bottom to top. The width of the last layer is
            // ignored, i.e. it always extend to the top atmosphere boundary. At most 2
            // layers can be specified.
            const std::vector<DensityProfileLayer>& rayleigh_density,
            // The scattering coefficient of air molecules at the altitude where their
            // density is maximum (usually the bottom of the atmosphere), as a function
            // of wavelength, in m^-1. The scattering coefficient at altitude h is equal
            // to 'rayleigh_scattering' times 'rayleigh_density' at this altitude. This
            // vector must have the same size as the wavelengths parameter.
            const std::vector<double>& rayleigh_scattering,
            // The density profile of aerosols, i.e. a function from altitude to
            // dimensionless values between 0 (null density) and 1 (maximum density).
            // Layers must be sorted from bottom to top. The width of the last layer is
            // ignored, i.e. it always extend to the top atmosphere boundary. At most 2
            // layers can be specified.
            const std::vector<DensityProfileLayer>& mie_density,
            // The scattering coefficient of aerosols at the altitude where their
            // density is maximum (usually the bottom of the atmosphere), as a function
            // of wavelength, in m^-1. The scattering coefficient at altitude h is equal
            // to 'mie_scattering' times 'mie_density' at this altitude. This vector
            // must have the same size as the wavelengths parameter.
            const std::vector<double>& mie_scattering,
            // The extinction coefficient of aerosols at the altitude where their
            // density is maximum (usually the bottom of the atmosphere), as a function
            // of wavelength, in m^-1. The extinction coefficient at altitude h is equal
            // to 'mie_extinction' times 'mie_density' at this altitude. This vector
            // must have the same size as the wavelengths parameter.
            const std::vector<double>& mie_extinction,
            // The asymetry parameter for the Cornette-Shanks phase function for the
            // aerosols.
            double mie_phase_function_g,
            // The density profile of air molecules that absorb light (e.g. ozone), i.e.
            // a function from altitude to dimensionless values between 0 (null density)
            // and 1 (maximum density). Layers must be sorted from bottom to top. The
            // width of the last layer is ignored, i.e. it always extend to the top
            // atmosphere boundary. At most 2 layers can be specified.
            const std::vector<DensityProfileLayer>& absorption_density,
            // The extinction coefficient of molecules that absorb light (e.g. ozone) at
            // the altitude where their density is maximum, as a function of wavelength,
            // in m^-1. The extinction coefficient at altitude h is equal to
            // 'absorption_extinction' times 'absorption_density' at this altitude. This
            // vector must have the same size as the wavelengths parameter.
            const std::vector<double>& absorption_extinction,
            // The average albedo of the ground, as a function of wavelength. This
            // vector must have the same size as the wavelengths parameter.
            const std::vector<double>& ground_albedo,
            // The maximum Sun zenith angle for which atmospheric scattering must be
            // precomputed, in radians (for maximum precision, use the smallest Sun
            // zenith angle yielding negligible sky light radiance values. For instance,
            // for the Earth case, 102 degrees is a good choice for most cases (120
            // degrees is necessary for very high exposure values).
            double max_sun_zenith_angle,
            // The length unit used in your shaders and meshes. This is the length unit
            // which must be used when calling the atmosphere model shader functions.
            double length_unit_in_meters,
            // The number of wavelengths for which atmospheric scattering must be
            // precomputed (the temporary GPU memory used during precomputations, and
            // the GPU memory used by the precomputed results, is independent of this
            // number, but the <i>precomputation time is directly proportional to this
            // number</i>):
            // - if this number is less than or equal to 3, scattering is precomputed
            // for 3 wavelengths, and stored as irradiance values. Then both the
            // radiance-based and the luminance-based API functions are provided (see
            // the above note).
            // - otherwise, scattering is precomputed for this number of wavelengths
            // (rounded up to a multiple of 3), integrated with the CIE color matching
            // functions, and stored as illuminance values. Then only the
            // luminance-based API functions are provided (see the above note).
            unsigned int num_precomputed_wavelengths,
            // Whether to pack the (red component of the) single Mie scattering with the
            // Rayleigh and multiple scattering in a single texture, or to store the
            // (3 components of the) single Mie scattering in a separate texture.
            bool combine_scattering_textures,
            // Whether to use half precision floats (16 bits) or single precision floats
            // (32 bits) for the precomputed textures. Half precision is sufficient for
            // most cases, except for very high exposure values.
            bool half_precision);

        ~Model();

        void Init(unsigned int num_scattering_orders = 4);

        GLuint shader() const { return atmosphere_shader_; }

        void SetProgramUniforms(
            GLuint program,
            GLuint transmittance_texture_unit,
            GLuint scattering_texture_unit,
            GLuint irradiance_texture_unit,
            GLuint optional_single_mie_scattering_texture_unit = 0) const;

        // Utility method to convert a function of the wavelength to linear sRGB.
        // 'wavelengths' and 'spectrum' must have the same size. The integral of
        // 'spectrum' times each CIE_2_DEG_COLOR_MATCHING_FUNCTIONS (and times
        // MAX_LUMINOUS_EFFICACY) is computed to get XYZ values, which are then
        // converted to linear sRGB with the XYZ_TO_SRGB matrix.
        static void ConvertSpectrumToLinearSrgb(
            const std::vector<double>& wavelengths,
            const std::vector<double>& spectrum,
            double* r, double* g, double* b);

        static constexpr double kLambdaR = 680.0;
        static constexpr double kLambdaG = 550.0;
        static constexpr double kLambdaB = 440.0;

    private:
        typedef std::array<double, 3> vec3;
        typedef std::array<float, 9> mat3;

        void Precompute(
            GLuint fbo,
            GLuint delta_irradiance_texture,
            GLuint delta_rayleigh_scattering_texture,
            GLuint delta_mie_scattering_texture,
            GLuint delta_scattering_density_texture,
            GLuint delta_multiple_scattering_texture,
            const vec3& lambdas,
            const mat3& luminance_from_radiance,
            bool blend,
            unsigned int num_scattering_orders);

        unsigned int num_precomputed_wavelengths_;
        bool half_precision_;
        bool rgb_format_supported_;
        std::function<std::string(const vec3&)> glsl_header_factory_;
        GLuint transmittance_texture_;
        GLuint scattering_texture_;
        GLuint optional_single_mie_scattering_texture_;
        GLuint irradiance_texture_;
        GLuint atmosphere_shader_;
        GLuint full_screen_quad_vao_;
        GLuint full_screen_quad_vbo_;
    };

}

class Atmosphere {
public:
    /*variables*/
    bool use_constant_solar_spectrum_;
    bool use_ozone_;
    bool use_combined_textures_;
    bool use_half_precision_;
    bool do_white_balance_;
    std::unique_ptr<atmosphere::Model>model_;

    GLuint vertex_shader_;
    GLuint fragment_shader_;
    GLuint program_;

    double view_distance_meters_;
    double view_zenith_angle_radians_;
    double view_azimuth_angle_radians_;
    double sun_zenith_angle_radians_;
    double sun_azimuth_angle_radians_;
    double exposure_;

    /*methods*/
    void initAtmosphere() {
        constexpr int kLambdaMin = 360;
        constexpr int kLambdaMax = 830;
        constexpr double kSolarIrradiance[48] = {
            1.11776, 1.14259, 1.01249, 1.14716, 1.72765, 1.73054, 1.6887, 1.61253,
            1.91198, 2.03474, 2.02042, 2.02212, 1.93377, 1.95809, 1.91686, 1.8298,
            1.8685, 1.8931, 1.85149, 1.8504, 1.8341, 1.8345, 1.8147, 1.78158, 1.7533,
            1.6965, 1.68194, 1.64654, 1.6048, 1.52143, 1.55622, 1.5113, 1.474, 1.4482,
            1.41018, 1.36775, 1.34188, 1.31429, 1.28303, 1.26758, 1.2367, 1.2082,
            1.18737, 1.14683, 1.12362, 1.1058, 1.07124, 1.04992
        };
        // Values from http://www.iup.uni-bremen.de/gruppen/molspec/databases/
        // referencespectra/o3spectra2011/index.html for 233K, summed and averaged in
        // each bin (e.g. the value for 360nm is the average of the original values
        // for all wavelengths between 360 and 370nm). Values in m^2.
        constexpr double kOzoneCrossSection[48] = {
            1.18e-27, 2.182e-28, 2.818e-28, 6.636e-28, 1.527e-27, 2.763e-27, 5.52e-27,
            8.451e-27, 1.582e-26, 2.316e-26, 3.669e-26, 4.924e-26, 7.752e-26, 9.016e-26,
            1.48e-25, 1.602e-25, 2.139e-25, 2.755e-25, 3.091e-25, 3.5e-25, 4.266e-25,
            4.672e-25, 4.398e-25, 4.701e-25, 5.019e-25, 4.305e-25, 3.74e-25, 3.215e-25,
            2.662e-25, 2.238e-25, 1.852e-25, 1.473e-25, 1.209e-25, 9.423e-26, 7.455e-26,
            6.566e-26, 5.105e-26, 4.15e-26, 4.228e-26, 3.237e-26, 2.451e-26, 2.801e-26,
            2.534e-26, 1.624e-26, 1.465e-26, 2.078e-26, 1.383e-26, 7.105e-27
        };
        // From https://en.wikipedia.org/wiki/Dobson_unit, in molecules.m^-2.
        constexpr double kDobsonUnit = 2.687e20;
        // Maximum number density of ozone molecules, in m^-3 (computed so at to get
        // 300 Dobson units of ozone - for this we divide 300 DU by the integral of
        // the ozone density profile defined below, which is equal to 15km).
        constexpr double kMaxOzoneNumberDensity = 300.0 * kDobsonUnit / 15000.0;
        // Wavelength independent solar irradiance "spectrum" (not physically
        // realistic, but was used in the original implementation).
        constexpr double kConstantSolarIrradiance = 1.5;
        constexpr double kBottomRadius = 6360000.0;
        constexpr double kTopRadius = 6420000.0;
        constexpr double kRayleigh = 1.24062e-6;
        constexpr double kRayleighScaleHeight = 8000.0;
        constexpr double kMieScaleHeight = 1200.0;
        constexpr double kMieAngstromAlpha = 0.0;
        constexpr double kMieAngstromBeta = 5.328e-3;
        constexpr double kMieSingleScatteringAlbedo = 0.9;
        constexpr double kMiePhaseFunctionG = 0.8;
        constexpr double kGroundAlbedo = 0.1;
        const double max_sun_zenith_angle =
            (use_half_precision_ ? 102.0 : 120.0) / 180.0 * kPi;

        atmosphere::DensityProfileLayer
            rayleigh_layer(0.0, 1.0, -1.0 / kRayleighScaleHeight, 0.0, 0.0);
        atmosphere::DensityProfileLayer mie_layer(0.0, 1.0, -1.0 / kMieScaleHeight, 0.0, 0.0);
        // Density profile increasing linearly from 0 to 1 between 10 and 25km, and
        // decreasing linearly from 1 to 0 between 25 and 40km. This is an approximate
        // profile from http://www.kln.ac.lk/science/Chemistry/Teaching_Resources/
        // Documents/Introduction%20to%20atmospheric%20chemistry.pdf (page 10).
        std::vector<atmosphere::DensityProfileLayer> ozone_density;
        ozone_density.push_back(
            atmosphere::DensityProfileLayer(25000.0, 0.0, 0.0, 1.0 / 15000.0, -2.0 / 3.0));
        ozone_density.push_back(
            atmosphere::DensityProfileLayer(0.0, 0.0, 0.0, -1.0 / 15000.0, 8.0 / 3.0));

        std::vector<double> wavelengths;
        std::vector<double> solar_irradiance;
        std::vector<double> rayleigh_scattering;
        std::vector<double> mie_scattering;
        std::vector<double> mie_extinction;
        std::vector<double> absorption_extinction;
        std::vector<double> ground_albedo;
        for (int l = kLambdaMin; l <= kLambdaMax; l += 10) {
            double lambda = static_cast<double>(l) * 1e-3;  // micro-meters
            double mie =
                kMieAngstromBeta / kMieScaleHeight * pow(lambda, -kMieAngstromAlpha);
            wavelengths.push_back(l);
            if (use_constant_solar_spectrum_) {
                solar_irradiance.push_back(kConstantSolarIrradiance);
            }
            else {
                solar_irradiance.push_back(kSolarIrradiance[(l - kLambdaMin) / 10]);
            }
            rayleigh_scattering.push_back(kRayleigh * pow(lambda, -4));
            mie_scattering.push_back(mie * kMieSingleScatteringAlbedo);
            mie_extinction.push_back(mie);
            absorption_extinction.push_back(use_ozone_ ?
                kMaxOzoneNumberDensity * kOzoneCrossSection[(l - kLambdaMin) / 10] :
                0.0);
            ground_albedo.push_back(kGroundAlbedo);
        }
        model_.reset(new atmosphere::Model(wavelengths, solar_irradiance, kSunAngularRadius,
            kBottomRadius, kTopRadius, { rayleigh_layer }, rayleigh_scattering,
            { mie_layer }, mie_scattering, mie_extinction, kMiePhaseFunctionG,
            ozone_density, absorption_extinction, ground_albedo, max_sun_zenith_angle,
            kLengthUnitInMeters, 15,
            use_combined_textures_, use_half_precision_));
        model_->Init();
        vertex_shader_ = glCreateShader(GL_VERTEX_SHADER);
        const char* const vertex_shader_source = kVertexShader;
        glShaderSource(vertex_shader_, 1, &vertex_shader_source, NULL);
        glCompileShader(vertex_shader_);

        const std::string fragment_shader_str =
            "#version 330\n" +
            std::string(use_luminance_ != NONE ? "#define USE_LUMINANCE\n" : "") +
            "const float kLengthUnitInMeters = " +
            std::to_string(kLengthUnitInMeters) + ";\n" +
            demo_glsl;
        const char* fragment_shader_source = fragment_shader_str.c_str();
        fragment_shader_ = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader_, 1, &fragment_shader_source, NULL);
        glCompileShader(fragment_shader_);

        if (program_ != 0) {
            glDeleteProgram(program_);
        }
        program_ = glCreateProgram();
        glAttachShader(program_, vertex_shader_);
        glAttachShader(program_, fragment_shader_);
        glAttachShader(program_, model_->shader());
        glLinkProgram(program_);
        glDetachShader(program_, vertex_shader_);
        glDetachShader(program_, fragment_shader_);
        glDetachShader(program_, model_->shader());

        /*
        <p>Finally, it sets the uniforms of this program that can be set once and for
        all (in our case this includes the <code>Model</code>'s texture uniforms,
        because our demo app does not have any texture of its own):
        */

        glUseProgram(program_);
        model_->SetProgramUniforms(program_, 0, 1, 2, 3);
        double white_point_r = 1.0;
        double white_point_g = 1.0;
        double white_point_b = 1.0;
        if (do_white_balance_) {
            Model::ConvertSpectrumToLinearSrgb(wavelengths, solar_irradiance,
                &white_point_r, &white_point_g, &white_point_b);
            double white_point = (white_point_r + white_point_g + white_point_b) / 3.0;
            white_point_r /= white_point;
            white_point_g /= white_point;
            white_point_b /= white_point;
        }
        glUniform3f(glGetUniformLocation(program_, "white_point"),
            white_point_r, white_point_g, white_point_b);
        glUniform3f(glGetUniformLocation(program_, "earth_center"),
            0.0, 0.0, -kBottomRadius / kLengthUnitInMeters);
        glUniform2f(glGetUniformLocation(program_, "sun_size"),
            tan(kSunAngularRadius),
            cos(kSunAngularRadius));


        //handle reshape event 
        //rewritten in glm 

        const float kFovY = glm::radians(50.0);
        float aspect = (float)(SCR_WDITH) / (float)SCR_HEIGHT;
        //glm::mat4 = glm::perspective()
    }

};
#endif  // ATMOSPHERE_MODEL_H_
