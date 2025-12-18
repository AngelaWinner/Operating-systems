package com.example.todoapi.repository;

import com.example.todoapi.model.Task;
import java.util.List;
import java.util.Optional;

public interface TaskRepository {

    List<Task> findAll();

    Task findById(int id);

    Task save(Task task);

    Task update(int id, Task task);

    boolean delete(int id);
}