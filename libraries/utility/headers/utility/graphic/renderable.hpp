/*
** ETIB PROJECT, 2026
** xider
** File description:
** renderable
*/

#pragma once

#include <memory>

#include <utility/graphic/color.hpp>
#include <utility/graphic/pose.hpp>
#include <utility/graphic/mesh.hpp>

namespace utility::graphic
{
    /**
     * @class Renderable
     * @brief The Renderable class represents an object that can be rendered in the graphics application.
     *
     * The Renderable class serves as a base class for objects that can be rendered, such as models, sprites, or other graphical entities. It provides a common interface and properties for renderable objects, allowing for consistent handling and rendering within the graphics application.
     */
    class Renderable
    {
        public:
        /**
         * @brief Virtual destructor for Renderable.
         */
        virtual ~Renderable() = default;

        /**
		 * @brief Set the text pose
		 *
         * @param pose Text world pose (position and orientation).
		 *
         * @return Reference to this Text instance for chaining.
		 */
		Renderable &setPose(const PoseF &pose);

        /**
         * @brief Set the text pose
         *
         * @param pose Text world pose (position and orientation).
         */
        void setPose(PoseF &&pose);

		/**
		 * @brief Get the text pose
		 *
         * @return Const reference to the text pose (position and orientation).
		 */
		const PoseF &getPose(void) const;

		/**
		 * @brief Set the text color.
		 *
         * @param color Text RGBA color.
		 *
         * @return Reference to this Text instance for chaining.
		 */
		Renderable &setColor(const graphic::Color32Bit &color);

        /**
         * @brief Set the text color.
         *
         * @param color Text RGBA color.
         */
        void setColor(graphic::Color32Bit &&color);

		/**
		 * @brief Get the text color.]
         *
		 * @return Const reference to the text color.
		 */
		const graphic::Color32Bit &getColor(void) const;

        protected:
		std::vector<std::shared_ptr<Mesh>> _meshes;
        Color32Bit _color;	   ///< Text RGBA color
		PoseF _pose;	///< World pose (position and orientation) of the text

    };
}	 // namespace utility::graphic