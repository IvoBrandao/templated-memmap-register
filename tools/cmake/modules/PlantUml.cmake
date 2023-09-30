

find_program(PLANTUML_EXECUTABLE plantuml)

if (PLANTUML_EXECUTABLE)
    message(STATUS "PlantUML found: ${PLANTUML_EXECUTABLE}")
    
    # Define a custom target to generate images from PlantUML files
    add_custom_target(generate_puml_images)

    # Find all *.puml files in the source directory
    file(GLOB_RECURSE PUML_FILES ${CMAKE_PROJECT_DIR}/*.puml)

    # Define the output directory for generated images (e.g., images/)
    set(OUTPUT_IMAGE_DIR ${CMAKE_BINARY_DIR}/arch)

    # Create the output directory if it doesn't exist
    file(MAKE_DIRECTORY ${OUTPUT_IMAGE_DIR})

    # Loop through each *.puml file and generate an image
    foreach(PUML_FILE ${PUML_FILES})
        get_filename_component(PUML_FILENAME ${PUML_FILE} NAME_WE)
        set(OUTPUT_IMAGE_FILE ${OUTPUT_IMAGE_DIR}/${PUML_FILENAME}.png)
        
        # Add a custom command to generate the image
        add_custom_command(
            OUTPUT ${OUTPUT_IMAGE_FILE}
            # change directory to the output directory
            WORKING_DIRECTORY ${OUTPUT_IMAGE_DIR}
            COMMAND plantuml ${PUML_FILE}
            DEPENDS ${PUML_FILE}
            COMMENT "Generating image: ${PUML_FILENAME}.png"
        )

        # Add the image to the custom target
        add_custom_target(${PUML_FILENAME}_image DEPENDS ${OUTPUT_IMAGE_FILE})
        add_dependencies(generate_puml_images ${PUML_FILENAME}_image)
    endforeach()

    # Add a phony target for generating all images
    add_custom_target(generate_all_puml_images DEPENDS generate_puml_images)
else ()
    message(WARNING "PlantUML not found. Images will not be generated.")
endif ()