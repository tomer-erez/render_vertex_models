I’ve reviewed the task thoroughly. Here’s a summary of what needs to be done, followed by a detailed plan tailored for gradual implementation:

### **Key Points of the Task:**
1. **Wireframe Renderer**:
   - Visualize geometric `.itd` data files in wireframe format.
   - Use a parser provided to read `.itd` files and traverse geometry data.
   - Store the parsed data in **custom data structures**.

2. **Matrix Transformations**:
   - Implement **rotation, scaling, and translation** for objects.
   - Accumulate transformations into a **single transformation matrix**.
   - Map object space to view space.

3. **Projections**:
   - Implement **orthographic** and/or **perspective projections**.
   - Map view space to screen space (current window).

4. **Line Drawing Algorithm**:
   - Implement a **line drawing algorithm** (floating-point or Bresenham).

5. **Advanced Features** (Optional for extra credit):
   - Object selection and individual transformations.
   - Mouse-based face highlighting.
   - Vertex normal calculation.

6. **Restrictions**:
   - No external code or libraries for vector/matrix math.
   - Implement custom classes for vectors, matrices, and operations.

---

### **Step-by-Step Plan**:

#### **Phase 1: Setup and Familiarization**
1. **Understand the Parser**:
   - Familiarize yourself with the parser and its API for `.itd` files.
   - Load and print parsed data to understand its structure (polygons, vertices, etc.).

2. **Setup the MFC GUI Skeleton**:
   - Ensure the MFC skeleton runs on your system (Visual Studio 2019 or 2022 recommended).
   - Understand how the skeleton handles rendering and interaction.

3. **Set Up Basic Data Structures**:
   - Implement basic classes for:
     - `Vector3`: For 3D points and operations.
     - `Matrix4x4`: For 4x4 transformation matrices.
     - `Polygon`: To store edges and vertices of parsed objects.
     - `Scene`: To store a collection of `Polygon` objects.

---

#### **Phase 2: Wireframe Rendering**
4. **Line Drawing Algorithm**:
   - Implement a **Bresenham line drawing algorithm**.
   - Integrate it into the MFC skeleton for drawing lines on the screen.

5. **Render a Static Wireframe**:
   - Use the parser to load a `.itd` file and populate your custom `Scene` structure.
   - Render the scene as a wireframe using your line drawing algorithm.

---

#### **Phase 3: Implement Transformations**
6. **Matrix Transformations**:
   - Implement basic matrix operations:
     - Multiplication of two 4x4 matrices.
     - Transforming a `Vector3` by a `Matrix4x4`.
   - Implement transformation matrices for:
     - **Translation**
     - **Scaling**
     - **Rotation** (around X, Y, and Z axes).

7. **Apply Transformations**:
   - Transform objects in the scene from **object space** to **view space**.
   - Test by applying translations, rotations, and scaling to all objects.

---

#### **Phase 4: Projections**
8. **Implement Orthographic Projection**:
   - Map transformed view-space coordinates to screen space using an orthographic projection.

9. **Implement Perspective Projection**:
   - Add support for perspective projection by mapping view-space coordinates with perspective division.

10. **Switch Between Projections**:
    - Add GUI controls (buttons or menu items) to toggle between orthographic and perspective projections.

---

#### **Phase 5: Advanced Features**
11. **Object Selection**:
    - Implement a way to select an object (e.g., by clicking or cycling through objects).
    - Apply transformations only to the selected object.

12. **Mouse-Based Face Highlighting**:
    - Map mouse clicks to screen-space coordinates.
    - Use the projections to detect faces under the mouse cursor.

13. **Vertex Normal Calculation**:
    - Maintain connectivity information for vertices.
    - Compute vertex normals as averages of incident face normals.

---

#### **Phase 6: Testing and Finalization**
14. **Test Edge Cases**:
    - Test for different `.itd` files with varying levels of complexity.
    - Ensure transformations and projections work consistently.

15. **Optimize and Comment Code**:
    - Optimize rendering and data structures.
    - Add detailed comments for all functions and classes.

16. **Prepare for Submission**:
    - Clean up the project.
    - Write a `README.txt` with your names, IDs, emails, and usage instructions.
    - Create a zip file with the required files.

---

### **What You Should Focus on Initially**:
- Implement basic vector and matrix math.
- Load and visualize `.itd` files with static wireframe rendering.
- Build confidence step by step before diving into transformations and projections.

Let me know how you’d like to proceed, and I can help you with any of these steps in more detail!